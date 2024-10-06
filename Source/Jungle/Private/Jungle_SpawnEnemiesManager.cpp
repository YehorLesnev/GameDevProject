// Fill out your copyright notice in the Description page of Project Settings.

#include "Jungle_SpawnEnemiesManager.h"
#include "Jungle_EnemySpawnTargetPoint.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJungle_SpawnEnemiesManager::AJungle_SpawnEnemiesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AJungle_SpawnEnemiesManager::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> TargetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJungle_EnemySpawnTargetPoint::StaticClass(), TargetPoints);	

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (AActor* TargetPoint : TargetPoints)
	{
		GetWorld()->SpawnActor<AActor>(BaseEnemyBP, TargetPoint->GetActorTransform(), SpawnParams);
	}
}

// Called every frame
void AJungle_SpawnEnemiesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
