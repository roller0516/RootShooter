// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ASGameState.generated.h"

/**
 * 
 */

class UASExperienceStateComponent;

UCLASS()
class AOS_API AASGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AASGameState();

	TObjectPtr<UASExperienceStateComponent> ExperienceState;
};
