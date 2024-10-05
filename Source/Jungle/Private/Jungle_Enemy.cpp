// Fill out your copyright notice in the Description page of Project Settings.


#include "Jungle_Enemy.h"

// Sets default values
AJungle_Enemy::AJungle_Enemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJungle_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJungle_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

