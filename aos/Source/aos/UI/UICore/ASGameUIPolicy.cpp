// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UICore/ASGameUIPolicy.h"
#include "GameCore/CommonGame/ASGameUIManagerSubSystem.h"
#include "GameCore/CommonGame/ASCommonLocalPlayer.h"
#include "ASPrimaryGameLayout.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "UObject/UObjectGlobals.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCommonGame, Log, All);
DEFINE_LOG_CATEGORY(LogCommonGame);

UASGameUIPolicy* UASGameUIPolicy::GetGameUIPolicy(const UObject* Worldcontext)
{
	if(UWorld* world = GEngine->GetWorldFromContextObject(Worldcontext,EGetWorldErrorMode::LogAndReturnNull))
	{
		if(UGameInstance* gameInstance = world->GetGameInstance())
		{
			if(UASGameUIManagerSubSystem* uimanager = gameInstance->GetSubsystem<UASGameUIManagerSubSystem>())
			{
				return uimanager->GetCurrnetGameUIPolicy();
			}
		}

	}
	return nullptr;
}

void UASGameUIPolicy::NotifyAddPlayer(UASCommonLocalPlayer* localPlayer)
{
	localPlayer->onPlayerControllerSet.AddWeakLambda(this, [this](UASCommonLocalPlayer* localPlayer, APlayerController* PlayerController)
	{
		NotifyRemovePlayer(localPlayer);

		if (FRootViewPortLayoutInfo* layoutInfo = ViewportLayoutInfos.FindByKey(localPlayer))
		{
			AddToViewPort(localPlayer, layoutInfo->primaryGameLayout);
			layoutInfo->bAddedViewPort = true;
		}
		else
		{
			CreateLayoutWidget(localPlayer);
		}
	});

	if (FRootViewPortLayoutInfo* layoutInfo = ViewportLayoutInfos.FindByKey(localPlayer))
	{
		AddToViewPort(localPlayer, layoutInfo->primaryGameLayout);
		layoutInfo->bAddedViewPort = true;
	}
	else
	{
		CreateLayoutWidget(localPlayer);
	}
}

void UASGameUIPolicy::NotifyRemovePlayer(UASCommonLocalPlayer* localPlayer)
{
	if(FRootViewPortLayoutInfo* layoutInfo = ViewportLayoutInfos.FindByKey(localPlayer))
	{
		RemoveLayoutFromViewport(localPlayer,layoutInfo->primaryGameLayout);
		layoutInfo->bAddedViewPort = false;
	
		//if(ViewportLayoutInfos.)
	}
}

void UASGameUIPolicy::NotifyDestroyPlayer(UASCommonLocalPlayer* localPlayer)
{
	NotifyRemovePlayer(localPlayer);
	localPlayer->onPlayerControllerSet.RemoveAll(this);
	int32 LayoutInfoIdx = ViewportLayoutInfos.IndexOfByKey(localPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		UASPrimaryGameLayout* Layout = ViewportLayoutInfos[LayoutInfoIdx].primaryGameLayout;
		ViewportLayoutInfos.RemoveAt(LayoutInfoIdx);

		RemoveLayoutFromViewport(localPlayer, Layout);
	}
}

UASPrimaryGameLayout* UASGameUIPolicy::GetRootLayout(const UASCommonLocalPlayer* LocalPlayer) const
{
	const FRootViewPortLayoutInfo* viewInfos = ViewportLayoutInfos.FindByKey(LocalPlayer);
	return viewInfos ? viewInfos->primaryGameLayout : nullptr;
}

void UASGameUIPolicy::AddToViewPort(UASCommonLocalPlayer* localPlayer, UASPrimaryGameLayout* layout)
{
	UE_LOG(LogCommonGame,Log,TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"),*GetName(),*GetNameSafe(localPlayer),*GetNameSafe(layout));

	layout->SetPlayerContext(FLocalPlayerContext(localPlayer));
	layout->AddToPlayerScreen(1000);
}

void UASGameUIPolicy::RemoveLayoutFromViewport(UASCommonLocalPlayer* localPlayer, UASPrimaryGameLayout* layout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = layout->GetCachedWidget();
	if(LayoutSlateWidget.IsValid())
	{
		UE_LOG(LogCommonGame, Log, TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"), *GetName(), *GetNameSafe(localPlayer), *GetNameSafe(layout));

		layout->RemoveFromParent();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(LogCommonGame, Log, TEXT("Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist. Noting in case we leak it."), *GetNameSafe(localPlayer), *GetNameSafe(layout));
		}
	}
}

void UASGameUIPolicy::CreateLayoutWidget(UASCommonLocalPlayer* localPlayer)
{
	if(APlayerController* playerController = localPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UASPrimaryGameLayout> layoutPrimaryLayout = GetLayoutWidgetClass(localPlayer);
		if(ensure(layoutPrimaryLayout))
		{
			UASPrimaryGameLayout* NewLayoutObject = CreateWidget<UASPrimaryGameLayout>(playerController,layoutPrimaryLayout);
			ViewportLayoutInfos.Add(FRootViewPortLayoutInfo(localPlayer, NewLayoutObject, true));
			AddToViewPort(localPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UASPrimaryGameLayout> UASGameUIPolicy::GetLayoutWidgetClass(UASCommonLocalPlayer* localPlayer)
{
	return LayoutClass.LoadSynchronous();
}
