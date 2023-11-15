// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "ASLoadingScreenManager.generated.h"

/**
 * 
 */
 
struct FWorldContext;
class FString;
class FSubsystemCollectionBase;
class UObject;
class FLoadingScreenInputPreProcessor;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoadingScreenVisibilityChanged, bool);

DECLARE_LOG_CATEGORY_EXTERN(LogLoadScreenManager, Log, All);
DEFINE_LOG_CATEGORY(LogLoadScreenManager);
UCLASS()
class AOS_API UASLoadingScreenManager : public UGameInstanceSubsystem , public FTickableGameObject
{
	GENERATED_BODY()
public:
	// USubSystem InterFace
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void HandlePreLoadMap(const FWorldContext& WorldContext, const FString& MapName);
	void HandlePostLoadMap(UWorld* LoadedWorld);

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
private:
	//Pre Load Map || Post Load Map 로드시 체크
	bool bCurrentlyInLoadMap = false;

	bool bCurrentlyShowingLoadingScreen = false;
	//Debug 용 메세지
	FString DebugLogMsg;

	float TimeLoadingScreenLastDismissed;

	float TimeLoadingScreen;

	TSharedPtr<FLoadingScreenInputPreProcessor> InputPreProcessor;

	TSharedPtr<SWidget> LoadingScreenWidget;

	FOnLoadingScreenVisibilityChanged OnLoadingScreenVisibilityChanged;
private:
	//Load Debug 용
	bool CheckForAnyNeedToShowLoadingScreen();

	bool ShouldShowLoadingScreen();

	void RemoveWidgetFromViewport();
	//로딩시 Input Blocking
	void StartBlockingInput();

	void StopBlockingInput();

	void ShowLoadingScreen();

	void HideLoadingScreen();

	void ChangePerformanceSettings(bool bEnabingLoadingScreen);
};
