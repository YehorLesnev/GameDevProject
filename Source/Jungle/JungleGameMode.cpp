// Copyright Epic Games, Inc. All Rights Reserved.

#include "JungleGameMode.h"
#include "Jungle_HUD.h"
#include "UObject/ConstructorHelpers.h"

AJungleGameMode::AJungleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/BP_Jungle_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AHUD> HUDClassFinder(TEXT("/Game/Characters/UI/BP_Jungle_HUD"));
	if (HUDClassFinder.Succeeded())
	{
		HUDClass = HUDClassFinder.Class;
	}
}
