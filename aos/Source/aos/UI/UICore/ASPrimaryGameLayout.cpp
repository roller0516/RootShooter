// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UICore/ASPrimaryGameLayout.h"
#include "Kismet/GameplayStatics.h"
#include "GameCore/CommonGame/ASCommonLocalPlayer.h"
#include "GameCore/CommonGame/ASGameUIManagerSubSystem.h"
#include "ASGameUIPolicy.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/Widgets/CommonActivatableWidgetContainer.h"

UASPrimaryGameLayout::UASPrimaryGameLayout(const FObjectInitializer& Initalizer)
	: Super(Initalizer)
{
}

UASPrimaryGameLayout* UASPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	APlayerController* PlayerController = GameInstance->GetPrimaryPlayerController(false);
	return GetPrimaryGameLayout(PlayerController);
}

UASPrimaryGameLayout* UASPrimaryGameLayout::GetPrimaryGameLayout(APlayerController* playercontroller)
{
	return playercontroller ? GetPrimaryGameLayout(Cast<UASCommonLocalPlayer>(playercontroller->Player)) : nullptr;
}

UASPrimaryGameLayout* UASPrimaryGameLayout::GetPrimaryGameLayout(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer)
	{
		const UASCommonLocalPlayer* CommonLocalPlayer = CastChecked<UASCommonLocalPlayer>(LocalPlayer);
		if (const UGameInstance* GameInstance = CommonLocalPlayer->GetGameInstance())
		{
			if (UASGameUIManagerSubSystem* UIManager = GameInstance->GetSubsystem<UASGameUIManagerSubSystem>())
			{
				if (const UASGameUIPolicy* Policy = UIManager->GetCurrnetGameUIPolicy())
				{
					if (UASPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CommonLocalPlayer))
					{
						return RootLayout;
					}
				}
			}
		}
	}

	return nullptr;
}

void UASPrimaryGameLayout::OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* Widget, bool bIsTransitioning)
{
    
}

void UASPrimaryGameLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
    if(!IsDesignTime())
    {   
        LayerWidget->OnTransitioningChanged.AddUObject(this,&UASPrimaryGameLayout::OnWidgetStackTransitioning);
        LayerWidget->SetTransitionDuration(0.0);

        Layers.Add(LayerTag,LayerWidget);
    }
}

void UASPrimaryGameLayout::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	for(const auto& Layer : Layers)
	{
		Layer.Value->RemoveWidget(*ActivatableWidget);
	}
}

UCommonActivatableWidgetContainerBase* UASPrimaryGameLayout::GetLayerWidget(FGameplayTag LayerName)
{
	return Layers.FindRef(LayerName);
}

