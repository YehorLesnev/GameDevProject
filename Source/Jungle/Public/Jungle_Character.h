#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Jungle_Character.generated.h"

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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* GunSkeletalMeshComponent;

	void MoveForward(float Axis);
	void MoveRight(float Axis);
	
	void Turn(float Axis);
	void LookUp(float Axis);

	void Sprint();
	void StopSprinting();

	void StartCrouch();
	void StopCrouch();
};