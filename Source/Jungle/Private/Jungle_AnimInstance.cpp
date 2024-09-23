// Fill out your copyright notice in the Description page of Project Settings.


#include "Jungle_AnimInstance.h" 
#include "Jungle_Character.h"
#include "Camera/CameraComponent.h" 

UJungle_AnimInstance::UJungle_AnimInstance()
{
	Character = nullptr;
	Mesh = nullptr;
	CurrentWeapon = nullptr;
}

void UJungle_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Character = Cast<AJungle_Character>(TryGetPawnOwner());

	if (!Character) return;

	Mesh = Character->GetMesh();
	Character->CurrentWeaponChangedDelegate.AddDynamic(this, &UJungle_AnimInstance::CurrentWeaponChanged);
	CurrentWeaponChanged(Character->CurrentWeapon, nullptr);
}

void UJungle_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!Character) return;

	SetVars(DeltaTime);
	CalculateWeaponSway(DeltaTime);
}

void UJungle_AnimInstance::SetVars(const float DeltaTime)
{
	CameraTransform = FTransform(Character->GetBaseAimRotation(), Character->Camera->GetComponentLocation());

	const FTransform& RootOffset = Mesh->GetSocketTransform(FName("root"), RTS_Component).Inverse() * Mesh->GetSocketTransform(FName("ik_hand_root"));
	RelativeCameraTransform = CameraTransform.GetRelativeTransform(RootOffset);
}

void UJungle_AnimInstance::CalculateWeaponSway(const float DeltaTime)
{
	
}

void UJungle_AnimInstance::CurrentWeaponChanged(AJungle_Weapon* NewWeapon, const AJungle_Weapon* OldWeapon)
{
	CurrentWeapon = NewWeapon;

	if (CurrentWeapon)
	{
		IKProperties = CurrentWeapon->IKProperties;
	}
}