// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Jungle_Weapon.h"
#include "Jungle_AnimInstance.generated.h"

UCLASS()
class JUNGLE_API UJungle_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UJungle_AnimInstance();
	
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void CurrentWeaponChanged(class AJungle_Weapon* NewWeapon, const class AJungle_Weapon* OldWeapon);
	virtual void SetVars(const float DeltaTime);
	virtual void CalculateWeaponSway(const float DeltaTime);
public:

	// REFERENCES
	UPROPERTY(BlueprintReadWrite, Category = "Anim")
		class AJungle_Character* Character;

	UPROPERTY(BlueprintReadWrite, Category = "Anim")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, Category = "Anim")
		class AJungle_Weapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FIKProperties IKProperties;

	// IK VARS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FTransform CameraTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		FTransform RelativeCameraTransform;
};
