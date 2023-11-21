// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASHUDLayout.h"
#include "NativeGameplayTags.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/UITag.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/Input/CommonUIInputTypes.h"
#include "GameCore/ASBluePrintUtil.h"
#include "aos.h"
#include "GameCore/ASGame/FASGamePlayTags.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/Input/CommonUIInputSettings.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");

UASHUDLayout::UASHUDLayout(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

void UASHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	FUIActionTag ActionTag = FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE);
	FBindUIActionArgs ActionArg = FBindUIActionArgs(ActionTag, false, FSimpleDelegate::CreateUObject(this, &ThisClass::EscapeActionHandle));

	RegisterUIActionBinding(ActionArg);

	for(const FUIActionBindingHandle& Handle : GetActionBindings())
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *Handle.GetActionName().ToString());
	}

	for(const FUIInputAction& action : UCommonUIInputSettings::Get().GetUIInputActions())
	{
		UE_LOG(LogTemp,Log, TEXT("%s"), *action.KeyMappings[0].Key.GetFName().ToString());
	}

	if (ActionArg.OnExecuteAction.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("Bind Yesssss"));
	}

}

void UASHUDLayout::EscapeActionHandle()
{
	if(ensure(!EscapeMenuClass.IsNull()))
	{
		UASBluePrintUtil::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU,EscapeMenuClass);
	}
}

void UASHUDLayout::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
