// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Jungle_HUD_Widget.generated.h"

/**
 * 
 */
UCLASS()
class JUNGLE_API UJungle_HUD_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
		void UpdateHealthBar(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
		void UpdateAmmoBar(float AmmoPercentage);
};
