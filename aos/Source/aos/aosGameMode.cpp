// Copyright Epic Games, Inc. All Rights Reserved.

#include "aosGameMode.h"
#include "aosCharacter.h"
#include "UObject/ConstructorHelpers.h"

AaosGameMode::AaosGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
