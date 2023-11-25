// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASBluePrintUtil.h"
#include "GameplayTagContainer.h"
#include "GameCore/CommonGame/ASGameUIManagerSubSystem.h"
#include "UI/UICore/ASPrimaryGameLayout.h"
#include "UI/UICore/ASGameUIPolicy.h"
#include "GameCore/CommonGame/ASCommonLocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "CommonUI/Public/CommonActivatableWidget.h"
#include "CommonInput/Public/CommonInputSubsystem.h"

int32 UASBluePrintUtil::InputSuspensions = 0;


UCommonActivatableWidget* UASBluePrintUtil::PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	if(UASGameUIManagerSubSystem* uiSubsystem = LocalPlayer->GetGameInstance()->GetSubsystem<UASGameUIManagerSubSystem>())
	{
		if(UASGameUIPolicy* uiPolicy = uiSubsystem->GetCurrnetGameUIPolicy())
		{
			if (UASPrimaryGameLayout* layout = uiPolicy->GetRootLayout(CastChecked<UASCommonLocalPlayer>(LocalPlayer)))
			{
				return layout->PushWidgetToLayerStack(LayerName, WidgetClass);
			}
		}
	}
	return nullptr;
}

void UASBluePrintUtil::PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
	if (UASGameUIManagerSubSystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UASGameUIManagerSubSystem>())
	{
		if (UASGameUIPolicy* Policy = UIManager->GetCurrnetGameUIPolicy())
		{
			if (UASPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UASCommonLocalPlayer>(LocalPlayer)))
			{
				const bool bSuspendInputUntilComplete = true;
				RootLayout->PushWidgetToLayerStackAsync(LayerName, bSuspendInputUntilComplete, WidgetClass);
			}
		}
	}
}

void UASBluePrintUtil::PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	if (const ULocalPlayer* LocalPlayer = ActivatableWidget->GetOwningLocalPlayer())
	{
		if (const UASGameUIManagerSubSystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UASGameUIManagerSubSystem>())
		{
			if (const UASGameUIPolicy* Policy = UIManager->GetCurrnetGameUIPolicy())
			{
				if (UASPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UASCommonLocalPlayer>(LocalPlayer)))
				{
					RootLayout->FindAndRemoveWidgetFromLayer(ActivatableWidget);
				}
			}
		}
	}
}

FName UASBluePrintUtil::SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason)
{
	return SuspendInputForPlayer(PlayerController ? PlayerController->GetLocalPlayer() : nullptr, SuspendReason);
}

FName UASBluePrintUtil::SuspendInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendReason)
{
	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		InputSuspensions++;
		FName SuspendToken = SuspendReason;
		SuspendToken.SetNumber(InputSuspensions);

		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, true);

		return SuspendToken;
	}

	return NAME_None;
}

void UASBluePrintUtil::ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken)
{
	ResumeInputForPlayer(PlayerController ? PlayerController->GetLocalPlayer() : nullptr, SuspendToken);
}

void UASBluePrintUtil::ResumeInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendToken)
{
	if (SuspendToken == NAME_None)
	{
		return;
	}

	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, false);
	}
}

