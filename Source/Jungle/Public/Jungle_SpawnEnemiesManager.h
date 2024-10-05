// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Jungle_Enemy.h"
#include "Jungle_SpawnEnemiesManager.generated.h"

UCLASS()
class JUNGLE_API AJungle_SpawnEnemiesManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJungle_SpawnEnemiesManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TSubclassOf<class AJungle_Enemy> BaseEnemyBP;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
