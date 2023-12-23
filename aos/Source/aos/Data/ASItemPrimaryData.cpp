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

TArray<FItemBaseData*> UASItemPrimaryData::GetRandomItemDatas()
{
	int32 num = FMath::FRandRange(1,101);

	TArray<FItemBaseData*> randomItemList;

	if(num <= 100)
	{
		int32 num2 = FMath::FRandRange(0,WeaponDatas.Num() - 1);

		randomItemList.Add(WeaponDatas[num2]);

		for (int i = 0; i < ItemDatas.Num(); i++)
		{
			randomItemList.Add(ItemDatas[i]);
		}
	}
	else
	{
		for (int i = 0; i < ItemDatas.Num(); i++)
		{
			randomItemList.Add(ItemDatas[i]);
		}
	}

	return randomItemList;
}

void UASItemPrimaryData::PostLoad()
{
	Super::PostLoad();

	CashedItemDataTable();
}

void UASItemPrimaryData::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	CashedItemDataTable();
}

void UASItemPrimaryData::CashedItemDataTable()
{
	if(ItemTable)
	{
		if(ItemDatas.Num() == 0)
		{
			TArray<FName> rowNames = ItemTable->GetRowNames();

			for (const FName& name : rowNames)
			{
				ItemDatas.AddUnique(ItemTable->FindRow<FItemBaseData>(name, name.ToString()));
			}
		}
	}
	
	if(WeaponTable)
	{
		if(WeaponDatas.Num() == 0)
		{
			TArray<FName>rowNames = WeaponTable->GetRowNames();

			for (FName& name : rowNames)
			{
				WeaponDatas.AddUnique(WeaponTable->FindRow<FWeaponData>(name, name.ToString()));
			}
		}
	}
}
