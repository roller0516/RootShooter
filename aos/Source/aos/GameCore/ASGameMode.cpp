// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "Data/ASUserFacingExperienceDefinition.h"
#include "Kismet/GameplayStatics.h"

void AASGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName,Options,ErrorMessage);
}

void AASGameMode::InitGameState()
{
	Super::InitGameState();
}
