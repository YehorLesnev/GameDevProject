// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Jungle_Bullet.generated.h"

UCLASS()
class JUNGLE_API AJungle_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJungle_Bullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Bullet speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
        float BulletSpeed;

    // Bullet damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
        float Damage;

    // Number of allowed ricochets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
        int32 MaxRicochets;
    
    UPROPERTY(VisibleAnywhere, Category = "Movement")
		USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Movement")
        UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
        UStaticMeshComponent* Mesh;

    // Function to handle ricochet logic
    void HandleRicochet(const FHitResult& HitResult);

	void FireInDirection(const FVector& ShootDirection);

protected:
    // Current ricochet count
    int32 CurrentRicochetCount;

    UFUNCTION()
        void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
