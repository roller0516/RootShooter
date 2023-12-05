// Fill out your copyright notice in the Description page of Project Settings.


#include "ASItemBase.h"

#include "Components/BoxComponent.h"
//#include "GameCore/ASGame/ASAssetManager.h"
#include "GameCore/ASGame/ASGameInstance.h"
#include "Data/ASItemPrimaryData.h"

// Sets default values

AASItemBase::AASItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	itemMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(itemMeshComponent);

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	collisionBox->SetupAttachment(itemMeshComponent);
}

// Called when the game starts or when spawned
void AASItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AASItemBase::SetTexture()
{
	IconItemTexture = itemBaseData.Texture;
}

void AASItemBase::SetMesh()
{
	
}

void AASItemBase::SetCount()
{
	
}

void AASItemBase::CreateItem(int32 _itemID)
{
	itemID = _itemID;

	UASGameInstance* instance = Cast<UASGameInstance>(GetWorld()->GetGameInstance());
	itemDataTable = instance->ItemData;

	if(itemDataTable->GetItemData(itemID))
	{
		itemBaseData = *itemDataTable->GetItemData(itemID);
		UpdateItem();
	}
}

void AASItemBase::UpdateItem()
{
	SetTexture();
	SetMesh();
	SetCount();
}
