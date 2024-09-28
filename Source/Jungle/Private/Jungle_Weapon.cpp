// Fill out your copyright notice in the Description page of Project Settings.


#include "Jungle_Weapon.h"
#include "Jungle_Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
AJungle_Weapon::AJungle_Weapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AJungle_Weapon::BeginPlay()
{
	Super::BeginPlay();

	if(!CurrentOwner)
		Mesh->SetVisibility(false);
}

void AJungle_Weapon::SingleFire()
{
	if (BulletClass && CurrentOwner)
	{
		// Declare some required params
		const float traceDistance = 5000.f;
		const bool drawDebugLine = false;
		const float calibrationDistance = 3000.f;

		APlayerController* PlayerController = Cast<APlayerController>(CurrentOwner->GetController());

		if (!PlayerController)
		{
			return;
		}

		// Get ViewRotation & Viewpoint	
		FVector viewDir = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
		viewDir.Normalize();

		FVector traceStart = PlayerController->PlayerCameraManager->GetCameraLocation();
		FVector traceEnd = traceStart + viewDir * traceDistance;

		// Set up trace params
		FCollisionQueryParams RV_TraceParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;
		RV_TraceParams.AddIgnoredActor(CurrentOwner);

		// Re-initialize hit info
		FHitResult RV_Hit(ForceInit);

		// Trace from center of the screen in the direction of the aim
		GetWorld()->LineTraceSingleByChannel(RV_Hit, traceStart, traceEnd, ECC_Visibility, RV_TraceParams);

		// Optionally draw the debug trace
		if (drawDebugLine)
		{
			DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Green, false, 1.0f);
		}

		// Get the muzzle location
		FVector muzzleLoc = Mesh->GetSocketLocation("BulletSpawnSocket");

		// The required direction of the projectile
		FVector locationToSendProjectileTo;

		// If we've hit something with the trace, let's send the projectile there.
		if (RV_Hit.bBlockingHit)
		{
			locationToSendProjectileTo = RV_Hit.ImpactPoint;
		}
		else
		{
			locationToSendProjectileTo = traceStart + viewDir * calibrationDistance;
		}

		// Calculate the bullet velocity direction
		FVector direction = (locationToSendProjectileTo - muzzleLoc).GetSafeNormal();

		// Draw debug line to show where bullet will go
		if (drawDebugLine)
		{
			DrawDebugLine(GetWorld(), muzzleLoc, locationToSendProjectileTo, FColor::Red, false, 1.0f);
		}

		// Get the muzzle rotation (the orientation of the bullet spawn)
		FRotator MuzzleRotation = direction.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		if (FireSound != nullptr && Owner != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());

		// Spawn the bullet at the muzzle location
		AJungle_Bullet* SpawnedBullet = GetWorld()->SpawnActor<AJungle_Bullet>(BulletClass, muzzleLoc, MuzzleRotation, SpawnParams);
		if (SpawnedBullet)
		{
			SpawnedBullet->SetOwner(CurrentOwner);
			SpawnedBullet->CollisionComponent->IgnoreActorWhenMoving(CurrentOwner, true);
			CurrentOwner->MoveIgnoreActorAdd(SpawnedBullet);
		 
			SpawnedBullet->FireInDirection(direction);
		}
	}
}

// Start automatic fire
void AJungle_Weapon::StartFire()
{
	if (IsAutomatic && !bIsFiring)  // Start firing only if not already firing
	{
		bIsFiring = true;

		// Immediately fire once, then start the timer to repeatedly fire
		HandleFiring();
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AJungle_Weapon::HandleFiring, FireRate, true);
	}
}

// Stop automatic fire
void AJungle_Weapon::StopFire()
{
	if (IsAutomatic)
	{
		bIsFiring = false;

		// Clear the timer to stop firing
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
}

// Handle the firing logic
void AJungle_Weapon::HandleFiring()
{
	// If no owner, or bullet class is not set, return early
	if (!BulletClass || !CurrentOwner)
	{
		StopFire();
		return;
	}

	// Fire a single shot
	SingleFire();
}

void AJungle_Weapon::Fire()
{
	if (IsAutomatic)
		return;

	SingleFire();
}