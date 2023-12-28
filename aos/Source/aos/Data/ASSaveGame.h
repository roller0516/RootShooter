// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ASSaveGame.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSoundOption
{
	GENERATED_BODY();

public:
	FSoundOption(){};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float UISoundVolum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SFXSoundVolum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float VolumeSoundVolum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MusicSoundVolum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float VoiceChatSoundVolum;
};

UCLASS()
class AOS_API UASSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FString SaveSlotName;		// Save game data file name

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int32 SaveIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MouseTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AimingMouseTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSoundOption soundOption;
};
