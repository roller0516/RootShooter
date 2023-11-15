// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "Interface/Command/ILoadingProcessInterFace.h"
#include "GameFeaturePluginOperationResult.h"
#include "ASGameFrameWorkComponent.generated.h"

enum class EFeatureLoadState : uint8
{
	Unload,
	Loading,
	LoadingGameFeature,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

class UASUserFacingExperienceDefinition;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOS_API UASGameFrameWorkComponent : public UGameStateComponent , public IILoadingProcessInterFace
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UASGameFrameWorkComponent(const FObjectInitializer& ObjectInitializer);
	//virtual void 

	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	//~ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~End of ILoadingProcessInterface

private:
	void StartLoad();
	void OnLoadComplete();
	void GameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void FullLoadComplete();

	EFeatureLoadState currentFeartureState;

	TObjectPtr<UASUserFacingExperienceDefinition> currentExperience;
};
