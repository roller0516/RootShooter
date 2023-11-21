// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeature/GameFeature_AddWidget.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Engine/AssetManager.h"
#include "Engine/Engine.h"
#include "UI/ASHUD.h"
#include "GameCore/ASBluePrintUtil.h"

void UGameFeature_AddWidget::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if(ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

void UGameFeature_AddWidget::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	Super::OnGameFeatureActivating(Context);
}

void UGameFeature_AddWidget::OnGameFeatureRegistering()
{
	Super::OnGameFeatureRegistering();
}

void UGameFeature_AddWidget::OnGameFeatureLoading()
{
	Super::OnGameFeatureLoading();
}

void UGameFeature_AddWidget::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* gameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if((gameInstance != nullptr) && (World != nullptr) &&World->IsGameWorld())
	{
		if(UGameFrameworkComponentManager* componentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(gameInstance))
		{
			TSoftClassPtr<AActor> HUDActorClass = AASHUD::StaticClass();

			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = componentManager->AddExtensionHandler(
				HUDActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &UGameFeature_AddWidget::HandleActorExtension, ChangeContext));
			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
	}
}

void UGameFeature_AddWidget::Reset(FPerContextData& ActiveData)
{
	ActiveData.ComponentRequests.Empty();
	ActiveData.LayoutsAdded.Empty();

	//for(FUIExtensionHandle)
	//ActiveData.ExtensionHandles.Reset();
}

void UGameFeature_AddWidget::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveWidgets(Actor, ActiveData);
	}
	else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) || (EventName == UGameFrameworkComponentManager::NAME_GameActorReady))
	{
		AddWidgets(Actor, ActiveData);
	}
}

void UGameFeature_AddWidget::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	AASHUD* HUD = CastChecked<AASHUD>(Actor);

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		for (const FHUDLayoutRequest& Entry : Layout)
		{
			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get())
			{
				ActiveData.LayoutsAdded.Add(UASBluePrintUtil::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass));
			}
		}

		//UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		//for (const FLyraHUDElementEntry& Entry : Widgets)
		//{
		//	ActiveData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
		//}
	}
}

void UGameFeature_AddWidget::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	AASHUD* HUD = CastChecked<AASHUD>(Actor);

	for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActiveData.LayoutsAdded)
	{
		if (AddedLayout.IsValid())
		{
			AddedLayout->DeactivateWidget();
		}
	}
	ActiveData.LayoutsAdded.Reset();
}
