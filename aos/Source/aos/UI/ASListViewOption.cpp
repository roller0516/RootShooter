// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ASListViewOption.h"

void UASListViewOption::NativeConstruct()
{
	Super::NativeConstruct();

	// Imagine we have an inventory class that provides us with the following:
	//TArray<UBUIInventoryItem*> Inventory;

	//for (UBUIInventoryItem* Item : Inventory)
	//{
	//	// Instantiate the widget
	//	UBUIInventoryEntry* Entry = CreateWidget<UBUIInventoryEntry>(this, EntryClass);

	//	// Set up its contents
	//	Entry->InitializeFromInventoryItem(Item);

	//	// Add it to the list
	//	EntriesVerticalBox->AddChildToVerticalBox(Entry);
	//}
}
