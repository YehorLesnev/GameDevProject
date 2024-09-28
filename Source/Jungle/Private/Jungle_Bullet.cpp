// Fill out your copyright notice in the Description page of Project Settings.


#include "Jungle_Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AJungle_Bullet::AJungle_Bullet()
{
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
		CollisionComponent->InitSphereRadius(15.0f);
		RootComponent = CollisionComponent;
	}

	// Initialize properties
	CurrentRicochetCount = 0;

	// Projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->bShouldBounce = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	//Mesh->OnComponentHit.AddDynamic(this, &AJungle_Bullet::OnBulletHit);  // Bind the OnHit function
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);  // For the bullet
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);  // Default block all
	CollisionComponent->BodyInstance.bUseCCD = true;

	// Enable sub-stepping to handle fast movement
	ProjectileMovementComponent->bForceSubStepping = true;
	ProjectileMovementComponent->MaxSimulationTimeStep = 0.005f;  // Increase accuracy with smaller time step
	ProjectileMovementComponent->MaxSimulationIterations = 16;    // Increase the number of iterations for simulation

	ProjectileMovementComponent->bSweepCollision = true;

	InitialLifeSpan = LifeSpan;
}

// Called when the game starts or when spawned
void AJungle_Bullet::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &AJungle_Bullet::OnBulletHit);  // Bind the OnHit function

	if (IsExplosive)
		GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AJungle_Bullet::Explode, InitialLifeSpan, false);
}

void AJungle_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpiralTrajectory)
	{
		FVector CurrentLocation = GetActorLocation();

		// Update position based on base velocity
		FVector BaseVelocity = ProjectileMovementComponent->Velocity * DeltaTime;
		FVector NextLocation = CurrentLocation + BaseVelocity;


		// Update spiral angle
		SpiralAngle += SpiralSpeed * DeltaTime; // Increment angle based on SpiralSpeed

		// Calculate offset for spiral motion
		float SpiralOffsetX = FMath::Cos(FMath::DegreesToRadians(SpiralAngle)) * SpiralRadius;
		float SpiralOffsetY = FMath::Sin(FMath::DegreesToRadians(SpiralAngle)) * SpiralRadius;

		// Add the spiral offset while maintaining the forward direction
		NextLocation += FVector(SpiralOffsetX, SpiralOffsetY, SpiralOffsetX); 


		SetActorLocation(NextLocation);
	}
}


void AJungle_Bullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetVelocity().GetSafeNormal(), Hit, GetInstigatorController(), this, nullptr);

		if (OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);

		if (CurrentRicochetCount < MaxRicochets)
		{
			HandleRicochet(Hit);
		}
		else
		{
			if (IsExplosive)
			{
				Explode();
			}

			Destroy();
		}
	}
}

void AJungle_Bullet::HandleRicochet(const FHitResult& HitResult)
{
	CurrentRicochetCount++;

	FVector SurfaceNormal = HitResult.ImpactNormal;
	FVector IncomingVelocity = ProjectileMovementComponent->Velocity;
	FVector ReflectionDirection = FMath::GetReflectionVector(IncomingVelocity, SurfaceNormal);

	// Apply new velocity through the ProjectileMovementComponent
	ProjectileMovementComponent->Velocity = ReflectionDirection * IncomingVelocity.Size();

	// Optionally rotate the bullet to match the new velocity direction
	FRotator NewRotation = ReflectionDirection.Rotation();
	SetActorRotation(NewRotation);
}

void AJungle_Bullet::FireInDirection(const FVector& bulletVelocity)
{
	// Set the bullet's velocity
	GetRootComponent()->ComponentVelocity = bulletVelocity;
}

void AJungle_Bullet::Explode()
{
	GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);

	FVector Location = GetActorLocation();

	// Spawn explosion particle effect
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, Location, FRotator::ZeroRotator, true);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}

	// Apply radial damage
	UGameplayStatics::ApplyRadialDamage(
		this,
		ExplosionDamage,
		Location,
		ExplosionRadius,
		nullptr,
		TArray<AActor*>(),  // Ignore actors
		this,
		GetInstigatorController(),
		true
	);

	// Find all actors in the explosion radius
	TArray<AActor*> OverlappingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		// Check if the actor has a primitive component and if it's simulating physics
		TArray<UPrimitiveComponent*> Components;
		Actor->GetComponents<UPrimitiveComponent>(Components);

		for (UPrimitiveComponent* Component : Components)
		{
			if (Component && Component->IsSimulatingPhysics())
			{
				// Calculate direction and apply impulse
				FVector Direction = Component->GetComponentLocation() - Location;
				Direction.Normalize();
				Component->AddImpulse(Direction * ExplosionForce, NAME_None, true);
			}
		}
	}
}