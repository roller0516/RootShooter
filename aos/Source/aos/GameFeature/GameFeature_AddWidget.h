// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "GameFeature/GameFeature_WorldActionBase.h"
#include "GameFeature_AddWidget.generated.h"

struct FComponentRequestHandle;

/**
 * 
 */

USTRUCT()
struct FHUDLayoutRequest
{
	GENERATED_BODY()

	// The layout widget to spawn
	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	// The layer to insert the widget in
	UPROPERTY(EditAnywhere, Category=UI, meta=(Categories="UI.Layer"))
	FGameplayTag LayerID;
};


USTRUCT()
struct FHUDElementEntry
{
	GENERATED_BODY()

	// The widget to spawn
	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	// The slot ID where we should place this widget
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;
};

struct FPerContextData
{
	TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
	TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;
};

UCLASS()
class AOS_API UGameFeature_AddWidget : public UGameFeature_WorldActionBase
{
	GENERATED_BODY()
public:
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureLoading() override;
private:
	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface
	void Reset(FPerContextData& ActiveData);
	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);
private:
	// Layout to add to the HUD
	UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{LayerID} -> {LayoutClass}"))
	TArray<FHUDLayoutRequest> Layout;

	// Widgets to add to the HUD
	UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{SlotID} -> {WidgetClass}"))
	TArray<FHUDElementEntry> Widgets;

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

};