// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ASItemPrimaryData.h"

FItemBaseData* UASItemPrimaryData::GetItemData(int32 itemID)
{
	return weaponTable->FindRow<FItemBaseData>(*FString::FromInt(itemID), TEXT(""));
}
