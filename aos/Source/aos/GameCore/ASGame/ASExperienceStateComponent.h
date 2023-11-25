// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "Interface/Command/ILoadingProcessInterFace.h"
#include "GameFeaturePluginOperationResult.h"
#include "ASExperienceStateComponent.generated.h"

class UASExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnExperienceLoaded, const UASExperienceDefinition*);

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOS_API UASExperienceStateComponent : public UGameStateComponent , public IILoadingProcessInterFace
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UASExperienceStateComponent(const FObjectInitializer& ObjectInitializer);
	//virtual void 

	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	//~ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~End of ILoadingProcessInterface

	void CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId primaryID,const FString& ExperienceIDSource);

	bool IsExperienceLoaded();

	FORCEINLINE UASExperienceDefinition* GetCurrentExperience(){return currentExperience;}
private:
	void StartLoad();
	void OnLoadComplete();
	void GameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void FullLoadComplete();
	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();
private:
	int NumExpectedPausers;

	int NumObservedPausers;

	int NumGameFeaturePluginsLoading;
	
	TArray<FString> GameFeaturePluginURLs;

	EFeatureLoadState currentFeartureState;

	TObjectPtr<UASExperienceDefinition> currentExperience;

	FOnExperienceLoaded OnExperienceLoaded;
};
