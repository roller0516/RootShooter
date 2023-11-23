// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ASListViewOptionItem.h"
#include "Components/TextBlock.h"

void UASListViewOptionItem::NativeConstruct()
{
	Super::NativeConstruct();

	NameBoxText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NmaeBoxText")));
	OptionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("OptionText")));
}

void UASListViewOptionItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	NameBoxText->SetText(FText::FromString("TEST NameBoxText"));
	OptionText->SetText(FText::FromString("TEST OptionText"));
}
