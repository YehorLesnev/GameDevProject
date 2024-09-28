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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
        FVector SpawnOffset;

    // Bullet damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
        float Damage;

    // Number of allowed ricochets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
        int32 MaxRicochets; 
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
        float LifeSpan = 3.0f;
    
    UPROPERTY(EditAnywhere, Category = "Explosion")
        bool IsExplosive;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
        UParticleSystem* ExplosionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
        USoundBase* ExplosionSound;

    UPROPERTY(EditAnywhere, Category = "Explosion")
        float ExplosionRadius = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Explosion")
        float ExplosionDamage = 50.0f;

    UPROPERTY(EditAnywhere, Category = "Explosion")
        float ExplosionForce = 2000.0f;

    UPROPERTY(VisibleAnywhere, Category = "Movement")
		USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Movement")
        UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
        UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spiral")
        bool bSpiralTrajectory = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spiral")
        float SpiralRadius = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spiral")
        float SpiralSpeed = 3000.0f;

    // Function to handle ricochet logic
    void HandleRicochet(const FHitResult& HitResult);

	void FireInDirection(const FVector& ShootDirection);

    void Explode();

protected:
    // Current ricochet count
    int32 CurrentRicochetCount;

    FTimerHandle ExplosionTimerHandle;

    float SpiralAngle = 0.0f;

    UFUNCTION()
        void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
