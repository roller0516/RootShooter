// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASInventoryComponent.h"
#include "../Item/ASItemBase.h"
#include "../Item/ASWeapon.h"

UASInventoryComponent::UASInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


bool UASInventoryComponent::CheckInventoryEquip()
{
	return EquipItems.Num() >= EQUIP_ITEM_CAPACITY;
}

void UASInventoryComponent::AddEquipItems(AASItemBase* weapon)
{
	EquipItems.Add(weapon);
	if (AASWeapon* asWeapon = Cast<AASWeapon>(weapon))
	{
		asWeapon->SetIsEquip(true);
	}
}

// Called when the game starts
void UASInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}