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
	AASGameState(const FObjectInitializer& ObjectInitializer);

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	TObjectPtr<UASExperienceStateComponent> ExperienceState;
};
