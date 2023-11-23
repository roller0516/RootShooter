// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASListViewOptionItem.h"

void UASListViewOptionItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	/*const APickupItem* Item = Cast<APickupItem>(ListItemObject);
	if (Item == nullptr)	return;

	ItemName->SetText(FText::FromString(Item->GetItemName()));*/ 
}
