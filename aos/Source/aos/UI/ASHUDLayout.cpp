// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASHUDLayout.h"
#include "NativeGameplayTags.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/UITag.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/Input/CommonUIInputTypes.h"
#include "GameCore/ASBluePrintUtil.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");

UASHUDLayout::UASHUDLayout(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

void UASHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE), false, FSimpleDelegate::CreateUObject(this, &ThisClass::EscapeActionHandle)));
}

void UASHUDLayout::EscapeActionHandle()
{
	if(ensure(!EscapeMenuClass.IsNull()))
	{
		UASBluePrintUtil::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU,EscapeMenuClass);
	}
}
