// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASHUDLayout.h"
#include "NativeGameplayTags.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/UITag.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/Input/CommonUIInputTypes.h"
#include "GameCore/ASBluePrintUtil.h"
#include "aos.h"
#include "GameCore/ASGame/FASGamePlayTags.h"



UASHUDLayout::UASHUDLayout(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

void UASHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(FASGameplayTags::Get().Tag_UI_Layer_Menu), false, FSimpleDelegate::CreateUObject(this, &ThisClass::EscapeActionHandle)));
}

void UASHUDLayout::EscapeActionHandle()
{
	if(ensure(!EscapeMenuClass.IsNull()))
	{
		UASBluePrintUtil::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), FASGameplayTags::Get().Tag_UI_Layer_Menu,EscapeMenuClass);
	}
}
