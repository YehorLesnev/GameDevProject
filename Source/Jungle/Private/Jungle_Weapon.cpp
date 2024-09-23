// Fill out your copyright notice in the Description page of Project Settings.


#include "Jungle_Weapon.h"

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

void AJungle_Weapon::Fire()
{
    if (BulletClass)
    {
        FVector CameraLocation;
        FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // Spawn bullet at weapon's muzzle location
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
        FRotator MuzzleRotation = CameraRotation;

        // Spawn bullet
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AJungle_Bullet* SpawnedBullet = GetWorld()->SpawnActor<AJungle_Bullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);

        if (SpawnedBullet)
        {
            // Set initial velocity of the bullet
            FVector LaunchDirection = MuzzleRotation.Vector();
			SpawnedBullet->FireInDirection(LaunchDirection);
        }
    }
}