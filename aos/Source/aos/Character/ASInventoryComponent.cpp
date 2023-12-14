// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASInventoryComponent.h"
#include "../Item/ASItemBase.h"
#include "../Item/ASWeapon.h"

UASInventoryComponent::UASInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	EquipItems.SetNum(EQUIP_ITEM_CAPACITY);
}


bool UASInventoryComponent::CheckInventoryEquip()
{
	return EquipItems.Num() >= EQUIP_ITEM_CAPACITY;
}

void UASInventoryComponent::SetEquipItemIndex(AASItemBase* weapon, int32 index)
{
	//EquipItems.Add(weapon);
	if (AASWeapon* asWeapon = Cast<AASWeapon>(weapon))
	{
		EquipItems[index] = weapon;
		asWeapon->SetIsEquip(true);
	}
}

// Called when the game starts
void UASInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}