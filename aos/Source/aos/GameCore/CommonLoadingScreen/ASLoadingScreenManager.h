// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "UObject/WeakInterfacePtr.h"
#include "ASLoadingScreenManager.generated.h"

/**
 * 
 */
 
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoadingScreenVisibilityChanged, bool);



struct FWorldContext;
class FString;
class FSubsystemCollectionBase;
class UObject;
class FLoadingScreenInputPreProcessor;
class IILoadingProcessInterFace;
class UGameInstance;

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
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual UWorld* GetTickableGameObjectWorld() const override;
	//IILoading Process Interface�� �ܺηκ��� ���
	void RegisterLoadingProcessInterFace(TScriptInterface<IILoadingProcessInterFace> loadingProcess);
	void UnRegisterLoadingProcessInterFace(TScriptInterface<IILoadingProcessInterFace> loadingProcess);

private:
	//Pre Load Map || Post Load Map �ε�� üũ
	bool bCurrentlyInLoadMap = false;

	bool bCurrentlyShowingLoadingScreen = false;
	//Debug �� �޼���
	FString DebugLogMsg;

	float TimeLoadingScreenLastDismissed;

	float TimeLoadingScreen;

	TSharedPtr<FLoadingScreenInputPreProcessor> InputPreProcessor;

	TSharedPtr<SWidget> LoadingScreenWidget;

	FOnLoadingScreenVisibilityChanged OnLoadingScreenVisibilityChanged;
	
	TArray<TWeakInterfacePtr<IILoadingProcessInterFace>> ExternalLoadingProcess;
private:
	//Load Debug ��
	bool CheckForAnyNeedToShowLoadingScreen();

	bool ShouldShowLoadingScreen();

	void RemoveWidgetFromViewport();
	//�ε��� Input Blocking
	void StartBlockingInput();

	void StopBlockingInput();

	void ShowLoadingScreen();

	void HideLoadingScreen();

	void ChangePerformanceSettings(bool bEnabingLoadingScreen);

	void UpdateLoadingScreen();
};
