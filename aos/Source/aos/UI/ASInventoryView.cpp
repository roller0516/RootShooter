// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASInventoryView.h"
#include "Components/CanvasPanel.h"
#include "CommonUI/Public/UITag.h"
#include "CommonUI/Public/Input/CommonUIInputTypes.h"
#include "GameCore/ASGame/ASBluePrintUtil.h"
#include "GameCore/ASGame/FASGamePlayTags.h"


UASInventoryView::UASInventoryView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	isShowEquip = true;
}

void UASInventoryView::ShowEquipView()
{
	isShowEquip = true;
	equipPanel->SetVisibility(ESlateVisibility::Visible);
	inventoryPanel->SetVisibility(ESlateVisibility::Hidden);
	RightPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UASInventoryView::ShowInventoryView()
{
	isShowEquip = false;
	inventoryPanel->SetVisibility(ESlateVisibility::Visible);
	equipPanel->SetVisibility(ESlateVisibility::Hidden);
	RightPanel->SetVisibility(ESlateVisibility::Visible);
}

void UASInventoryView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FUIActionTag ActionTag = FUIActionTag::ConvertChecked(FASGameplayTags::Get().Tag_UI_Action_Escape);
	FBindUIActionArgs ActionArg = FBindUIActionArgs(ActionTag, false, FSimpleDelegate::CreateUObject(this, &UASInventoryView::EscapeActionHandle));
	RegisterUIActionBinding(ActionArg);
}

void UASInventoryView::EscapeActionHandle()
{
	if(!isShowEquip)
	{
		ShowEquipView();
	}
	else
	{
		UASBluePrintUtil::PopContentFromLayer(this);
	}
}
