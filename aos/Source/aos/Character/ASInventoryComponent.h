// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASInventoryComponent.generated.h"

class AASItemBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOS_API UASInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UASInventoryComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = Inven, meta = (AllowPrivateAccess = "true"))
	TArray<AASItemBase*> Items;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = Inven, meta = (AllowPrivateAccess = "true"))
	TArray<AASItemBase*> EquipItems;

	const int32 EQUIP_ITEM_CAPACITY = 3;

	bool CheckInventoryEquip();

	void AddEquipItems(AASItemBase* weapon);

	FORCEINLINE void AddItems(AASItemBase* Item)
	{
		Items.Add(Item);
	}
protected:
	virtual void BeginPlay() override;
};
