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
    BulletSpeed = 3000.0f;
    Damage = 25.0f;
    MaxRicochets = 3;
    CurrentRicochetCount = 0;

    // Projectile movement component
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
    ProjectileMovementComponent->InitialSpeed = BulletSpeed;
    ProjectileMovementComponent->MaxSpeed = BulletSpeed;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.3f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.001f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AJungle_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJungle_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJungle_Bullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Apply damage if we hit an actor
    if (OtherActor && OtherActor != this && OtherComp)
    {
        UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetVelocity().GetSafeNormal(), Hit, GetInstigatorController(), this, nullptr);

        // Handle ricochet behavior
        if (CurrentRicochetCount < MaxRicochets)
        {
            HandleRicochet(Hit);
        }
        else
        {
            // Destroy the bullet if max ricochets are reached
            Destroy();
        }
    }
}

void AJungle_Bullet::HandleRicochet(const FHitResult& HitResult)
{
    CurrentRicochetCount++;

    // Get the surface normal from the hit result
    FVector SurfaceNormal = HitResult.ImpactNormal;

    // Calculate reflection direction (ricochet)
    FVector IncomingVelocity = GetVelocity();
    FVector ReflectionDirection = FMath::GetReflectionVector(IncomingVelocity, SurfaceNormal);

    // Normalize the reflection direction and apply bullet speed
    FVector NewVelocity = ReflectionDirection.GetSafeNormal() * BulletSpeed;

    // Set new velocity for the bullet
    GetRootComponent()->ComponentVelocity = NewVelocity;

    // Rotate the bullet to face its new direction
    FRotator NewRotation = NewVelocity.Rotation();
    SetActorRotation(NewRotation);
}

void AJungle_Bullet::FireInDirection(const FVector& ShootDirection)
{
	// Set the bullet's velocity
	GetRootComponent()->ComponentVelocity = ShootDirection * BulletSpeed;
}