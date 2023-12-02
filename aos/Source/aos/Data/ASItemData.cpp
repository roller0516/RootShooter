// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ASItemData.h"
#include "ASItemPrimaryData.h"

FItemBaseData* UASItemData::GetItemData(int32 itemID)
{
	return weaponTable->FindRow<FItemBaseData>(*FString::FromInt(itemID), TEXT(""));
}
