// Fill out your copyright notice in the Description page of Project Settings.


#include "ASItemBase.h"

#include "Components/BoxComponent.h"
//#include "GameCore/ASGame/ASAssetManager.h"
#include "GameCore/ASGame/ASGameInstance.h"
#include "Data/ASItemPrimaryData.h"
#include "Components/BoxComponent.h"
#include "GameCore/ASGame/ASAssetManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values

AASItemBase::AASItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	itemMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(itemMeshComponent);

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	collisionBox->SetupAttachment(itemMeshComponent);
}

void AASItemBase::PickupItem()
{	
	//itemMeshComponent->SetHiddenInGame(true);
	SetActorHiddenInGame(true);
	collisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
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
	
	if(GetWorld())
	{
		UASGameInstance* instance = Cast<UASGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		itemDataTable = instance->ItemData;
	}
	else
	{
		UASAssetManager& assetManager = UASAssetManager::Get();
		itemDataTable = assetManager.GetPrimaryData<UASItemPrimaryData>(FPrimaryAssetId(FPrimaryAssetType("ASItemData"), FName("ItemData")));
	}
	
	if(itemDataTable->GetItemData(itemID))
	{
		itemBaseData = *itemDataTable->GetItemData(itemID);
		UpdateItem();
	}
}

void AASItemBase::CopyItem(AASItemBase* item)
{
	itemID = item->itemID;
	if (GetWorld())
	{
		UASGameInstance* instance = Cast<UASGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		itemDataTable = instance->ItemData;
	}
	else
	{
		UASAssetManager& assetManager = UASAssetManager::Get();
		itemDataTable = assetManager.GetPrimaryData<UASItemPrimaryData>(FPrimaryAssetId(FPrimaryAssetType("ASItemData"), FName("ItemData")));
	}

	if (itemDataTable->GetItemData(itemID))
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

void AASItemBase::OnConstruction(const FTransform& Transform)
{
	UASAssetManager& assetManager = UASAssetManager::Get();
	itemDataTable = assetManager.GetPrimaryData<UASItemPrimaryData>(FPrimaryAssetId(FPrimaryAssetType("ASItemData"), FName("ItemData")));

	if (itemDataTable->GetItemData(itemID))
	{
		itemBaseData = *itemDataTable->GetItemData(itemID);
		UpdateItem();
	}
}

void AASItemBase::RefreshItem()
{
	
}

