// Fill out your copyright notice in the Description page of Project Settings.


#include "ASItemBase.h"

#include "Components/BoxComponent.h"
#include "GameCore/ASGame/ASAssetManager.h"
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

AASItemBase::AASItemBase(int32 itemID)
	: itemID(itemID)
{
	
}

// Called when the game starts or when spawned
void AASItemBase::BeginPlay()
{
	Super::BeginPlay();

	UASAssetManager& asssetmanager = UASAssetManager::Get();
	itemDataTable = asssetmanager.GetPrimaryData<UASItemPrimaryData>(FPrimaryAssetId(FPrimaryAssetType("ASItemData"), FName("ItemData")));

	UpdateItemBaseData();
}

void AASItemBase::SetTexture()
{
	
}

void AASItemBase::SetMesh()
{
	
}

void AASItemBase::SetCount()
{
	
}

void AASItemBase::UpdateItemBaseData()
{
	SetTexture();
	SetMesh();
	SetCount();
}
