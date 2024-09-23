#include "Jungle_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Jungle_Weapon.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AJungle_Character::AJungle_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera setup
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	check(Camera != nullptr);
	Camera->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), FName("Head"));
	 
	// Movement setup
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	// Gun setup
	/*GunSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Skeletal Mesh"));
	GunSkeletalMeshComponent->bCastDynamicShadow = false;
	GunSkeletalMeshComponent->CastShadow = false;
	GunSkeletalMeshComponent->SetupAttachment(Camera);*/
}

// Called when the game starts or when spawned
void AJungle_Character::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		for (const TSubclassOf<AJungle_Weapon>& WeaponClass : DefaultWeapons)
		{
			if (!WeaponClass) continue;
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			AJungle_Weapon* SpawnedWeapon = GetWorld()->SpawnActor<AJungle_Weapon>(WeaponClass, SpawnParams);
			const int32 Index = Weapons.Add(SpawnedWeapon);

			if (Index == CurrentIndex)
			{
				CurrentWeapon = SpawnedWeapon;
				OnRep_CurrentWeapon(nullptr);
			}
		}
	}

	//GunSkeletalMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("WeaponGripPoint"));
}

// Called every frame
void AJungle_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJungle_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AJungle_Character::Jump);
	PlayerInputComponent->BindAxis("MoveForward", this, &AJungle_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJungle_Character::MoveRight);
	PlayerInputComponent->BindAxis("TurnCamera", this, &AJungle_Character::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AJungle_Character::LookUp);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AJungle_Character::Sprint);
	PlayerInputComponent->BindAction("StopSprinting", IE_Released, this, &AJungle_Character::StopSprinting);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AJungle_Character::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AJungle_Character::StopCrouch);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &AJungle_Character::NextWeapon);
	PlayerInputComponent->BindAction("LastWeapon", IE_Pressed, this, &AJungle_Character::LastWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AJungle_Character::Fire);
}

void AJungle_Character::OnRep_CurrentWeapon(const AJungle_Weapon* OldWeapon)
{
	if (CurrentWeapon)
	{
		if (!CurrentWeapon->CurrentOwner) 
		{
			const FTransform& PlacementTransform = CurrentWeapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("Weapon_R"));

			CurrentWeapon->SetActorTransform(PlacementTransform, false, nullptr, ETeleportType::TeleportPhysics);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("Weapon_R"));
			
			CurrentWeapon->CurrentOwner = this;
		}

		CurrentWeapon->Mesh->SetVisibility(true);
	}

	if (OldWeapon)
	{
		OldWeapon->Mesh->SetVisibility(false);
	}

	CurrentWeaponChangedDelegate.Broadcast(CurrentWeapon, OldWeapon);
}

void AJungle_Character::Server_SetCurrentWeapon_Implementation(AJungle_Weapon* Weapon)
{
	const AJungle_Weapon* OldWeapon = CurrentWeapon;
	CurrentWeapon = Weapon;
	OnRep_CurrentWeapon(OldWeapon);
}

void AJungle_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AJungle_Character, Weapons, COND_None);
	DOREPLIFETIME_CONDITION(AJungle_Character, CurrentWeapon, COND_None);
}

void AJungle_Character::EquipWeapon(const int32 Index)
{
	if (!Weapons.IsValidIndex(Index) || CurrentWeapon == Weapons[Index]) return;

	if (IsLocallyControlled())
	{
		const AJungle_Weapon* OldWeapon = CurrentWeapon;
		CurrentIndex = Index;
		CurrentWeapon = Weapons[Index];
		OnRep_CurrentWeapon(OldWeapon);
	}
	else if(!HasAuthority())
	{
		Server_SetCurrentWeapon_Implementation(Weapons[Index]);
	}
}

void AJungle_Character::NextWeapon()
{
	const int32 Index = Weapons.IsValidIndex(CurrentIndex + 1) ? CurrentIndex + 1 : 0;
	EquipWeapon(Index);
}

void AJungle_Character::LastWeapon()
{
	const int32 Index = Weapons.IsValidIndex(CurrentIndex - 1) ? CurrentIndex - 1 : Weapons.Num() - 1;
	EquipWeapon(Index);
}

void AJungle_Character::MoveForward(float Axis)
{
	FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, Axis);
}

void AJungle_Character::MoveRight(float Axis)
{
	FVector Right = GetActorRightVector();
	AddMovementInput(Right, Axis);
}

void AJungle_Character::Turn(float Axis)
{
	AddControllerYawInput(Axis);
}

void AJungle_Character::LookUp(float Axis)
{
	AddControllerPitchInput(Axis);
}

void AJungle_Character::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void AJungle_Character::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AJungle_Character::StartCrouch()
{
	GetCharacterMovement()->bWantsToCrouch = true;
	GetCharacterMovement()->Crouch();
}

void AJungle_Character::StopCrouch()
{
	GetCharacterMovement()->bWantsToCrouch = false;
	GetCharacterMovement()->UnCrouch();
}
//
//void AJungle_Character::FireWeapon()
//{
//	if (CurrentWeapon)
//		CurrentWeapon->Fire();
//}


void AJungle_Character::Fire()
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