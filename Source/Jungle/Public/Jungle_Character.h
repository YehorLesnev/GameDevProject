#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Jungle_Bullet.h"
#include "Jungle_Weapon.h"
#include "Jungle_HUD_Widget.h"
#include "Blueprint/UserWidget.h"
#include "Jungle_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireDelegate, float, AmmoPercentage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrectWeaponChangedDelegate, class AJungle_Weapon*, CurrentWeapon, const class AJungle_Weapon*, OldWeapon);

UCLASS()
class JUNGLE_API AJungle_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJungle_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category = "State")
		TArray<class AJungle_Weapon*> Weapons;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
		int32 CurrentIndex = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentWeapon, Category = "State")
		class AJungle_Weapon* CurrentWeapon;

	// callsed whenever the current weapon is changed
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FCurrectWeaponChangedDelegate CurrentWeaponChangedDelegate;

	UFUNCTION(BlueprintCallable, Category = "Cahracter")
		virtual void EquipWeapon(const int32 Index);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnFireDelegate OnFire;

	// Widget class to use for HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		TSubclassOf<UJungle_HUD_Widget> HUDWidgetClass;

	// Pointer to the active HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		UJungle_HUD_Widget* HUDWidget;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
		bool IsReloading;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxHealth = 100.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Stats")
		float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
		TArray<TSubclassOf<class AJungle_Weapon>> DefaultWeapons;

	UFUNCTION(Server, Reliable)
		void Server_SetCurrentWeapon(class AJungle_Weapon* Weapon);

	UFUNCTION()
		virtual void OnRep_CurrentWeapon(const class AJungle_Weapon* OldWeapon);

	virtual void Server_SetCurrentWeapon_Implementation(class AJungle_Weapon* Weapon);
	virtual void NextWeapon();
	virtual void LastWeapon();

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	void Turn(float Axis);
	void LookUp(float Axis);

	void Sprint();
	void StopSprinting();

	void StartCrouch();
	void StopCrouch();

	void Fire();
	void StartFiringWeapon();
	void StopFiringWeapon();

	void Reload();
};