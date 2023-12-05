// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ASItemPrimaryData.h"

FItemBaseData* UASItemPrimaryData::GetItemData(int32 itemID)
{
	for(int i = 0 ; i < ItemDatas.Num(); i++)
	{
		if(ItemDatas[i]->ItemID == itemID)
		{
			return ItemDatas[i];
		}
	}
	return nullptr;
}

FWeaponData* UASItemPrimaryData::GetWeaponData(int32 itemID)
{
	for (int i = 0; i < WeaponDatas.Num(); i++)
	{
		if (WeaponDatas[i]->ItemID == itemID)
		{
			return WeaponDatas[i];
		}
	}
	return nullptr;
}

void UASItemPrimaryData::PostLoad()
{
	Super::PostLoad();

	CashedItemDataTable();
}

void UASItemPrimaryData::CashedItemDataTable()
{
	if(ItemTable)
	{
		TArray<FName> rowNames = ItemTable->GetRowNames();

		for (const FName& name : rowNames)
		{
			ItemDatas.Add(ItemTable->FindRow<FItemBaseData>(name, name.ToString()));
		}
	}
	
	if(WeaponTable)
	{
		TArray<FName>rowNames = WeaponTable->GetRowNames();

		for (FName& name : rowNames)
		{
			WeaponDatas.Add(WeaponTable->FindRow<FWeaponData>(name, name.ToString()));
		}
	}
	
}
