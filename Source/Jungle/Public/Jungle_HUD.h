// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Jungle_HUD.generated.h"

/**
 * 
 */
UCLASS()
class JUNGLE_API AJungle_HUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void DrawHUD() override;

private:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* CrosshairTex;

};
