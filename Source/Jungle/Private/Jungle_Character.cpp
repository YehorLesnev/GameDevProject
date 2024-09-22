#include "Jungle_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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
	GunSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Skeletal Mesh"));
	GunSkeletalMeshComponent->bCastDynamicShadow = false;
	GunSkeletalMeshComponent->CastShadow = false;
	GunSkeletalMeshComponent->SetupAttachment(Camera);
}

// Called when the game starts or when spawned
void AJungle_Character::BeginPlay()
{
	Super::BeginPlay();

	GunSkeletalMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("WeaponGripPoint"));
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