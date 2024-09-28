// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Jungle_Bullet.h"
#include "Jungle_Weapon.generated.h"


USTRUCT(BlueprintType)
struct FIKProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class UAnimSequence* AnimPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float AnimOffset = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FTransform CustomOffsetTransform;
};

UCLASS(Abstract)
class JUNGLE_API AJungle_Weapon : public AActor
{
	GENERATED_BODY()

public:
	AJungle_Weapon();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
		class AJungle_Character* CurrentOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		FIKProperties IKProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		bool IsAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		float FireRate = 0.1f;  // Fire rate in seconds (0.1 = 10 rounds per second)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		FTransform PlacementTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		USoundBase* FireSound;

	// Fire control
	FTimerHandle FireTimerHandle; // Timer handle for automatic fire
	bool bIsFiring = false; // To track if the weapon is firing

	void Fire();
	void StartFire();
	void StopFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AJungle_Bullet> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FVector MuzzleOffset;


private:
	void HandleFiring();

	void SingleFire();
};
