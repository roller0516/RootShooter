// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASHUDLayout.h"
#include "NativeGameplayTags.h"
#include "CommonUI/Public/UITag.h"
#include "CommonUI/Public/Input/CommonUIInputTypes.h"
#include "GameCore/ASGame/ASBluePrintUtil.h"
#include "aos.h"
#include "GameCore/ASGame/FASGamePlayTags.h"
#include "CommonUI/Public/Input/CommonUIInputSettings.h"
#include "CommonUI/Public/Input/UIActionBinding.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");

UASHUDLayout::UASHUDLayout(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

void UASHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	FUIActionTag ActionTag = FUIActionTag::ConvertChecked(FASGameplayTags::Get().Tag_UI_Action_Escape);
	FBindUIActionArgs ActionArg = FBindUIActionArgs(ActionTag, false, FSimpleDelegate::CreateUObject(this, &UASHUDLayout::EscapeActionHandle));
	RegisterUIActionBinding(ActionArg);
}

void UASHUDLayout::EscapeActionHandle()
{
	if(ensure(!EscapeMenuClass.IsNull()))
	{
		UASBluePrintUtil::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU,EscapeMenuClass);
	}
}
