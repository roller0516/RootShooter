// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASWorldSettings.h"
#include "Engine/AssetManager.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Misc/UObjectToken.h"
#include "aos.h"

AASWorldSettings::AASWorldSettings(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void AASWorldSettings::CheckForErrors()
{
	Super::CheckForErrors();
	
	FMessageLog MapChek("MapCheck");
	
	for(TActorIterator<APlayerStart> PlayerStartIt(GetWorld()); PlayerStartIt; ++PlayerStartIt)
	{
		APlayerStart* PlayerStart = *PlayerStartIt;
		if(IsValid(PlayerStart) && PlayerStart->GetClass() == APlayerStart::StaticClass())
		{
			MapChek.Warning()
				->AddToken(FUObjectToken::Create(PlayerStart))
				->AddToken(FTextToken::Create(FText::FromString("is a normal APlayerStart, replace with ALyraPlayerStart.")));
		}
	}
}

FPrimaryAssetId AASWorldSettings::GetDefaultGameplayExperience() const
{
	FPrimaryAssetId Result;
	if(!DefaultGameplayExperience.IsNull())
	{
		Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());

		if(!Result.IsValid())
		{
			UE_LOG(LogASExperience, Error, TEXT("%s.DefaultGameplayExperience is %s but that failed to resolve into an asset ID (you might need to add a path to the Asset Rules in your game feature plugin or project settings"),
				*GetPathNameSafe(this), *DefaultGameplayExperience.ToString());
		}
	}

	return Result;
}
