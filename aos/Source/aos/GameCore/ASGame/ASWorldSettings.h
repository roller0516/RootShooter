// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "ASWorldSettings.generated.h"

/**
 * 
 */
class UASExperienceDefinition;

UCLASS()
class AOS_API AASWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
	AASWorldSettings(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	void CheckForErrors() override;
#endif
public:
	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	TSoftClassPtr<UASExperienceDefinition> DefaultGameplayExperience;
};
