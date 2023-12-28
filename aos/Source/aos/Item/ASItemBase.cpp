// Fill out your copyright notice in the Description page of Project Settings.


#include "ASItemBase.h"

#include "Components/BoxComponent.h"
//#include "GameCore/ASGame/ASAssetManager.h"
#include "GameCore/ASGame/ASGameInstance.h"
#include "Data/ASItemPrimaryData.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameCore/ASGame/ASAssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values

AASItemBase::AASItemBase()
{
	bFalling = false;
	BounceItemTime = 0.7f;

	PrimaryActorTick.bCanEverTick = false;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	itemMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	//itemMeshComponent->SetSimulatePhysics(false);
	itemMeshComponent->SetupAttachment(root);

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("static Mesh"));
	//staticMesh->SetSimulatePhysics(false);
	staticMesh->SetupAttachment(root);
	
	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	collisionBox->SetupAttachment(root);
	
	particle = CreateDefaultSubobject<USceneComponent>(TEXT("Particle"));
	particle->SetupAttachment(root);
	FTransform tr;
	tr.SetRotation(FRotator(90,0,0).Quaternion());
	particle->SetRelativeTransform(tr);
}

void AASItemBase::SetItemProperties(E_ItemState state)
{
	currentState = state;

	switch (currentState)
	{
	case E_ItemState::E_None:
		//collisionBox->SetcollisionRes
		collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
		collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
		break;
	case E_ItemState::E_Drop:
		if(itemBaseData.mesh)
		{
			//itemMeshComponent->SetSimulatePhysics(true);
			//itemMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			itemMeshComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
			itemMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
			itemMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
		}
		else
		{
			//staticMesh->SetSimulatePhysics(true);
			//staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			staticMesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
			staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
			staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
		}

		//BounceItem();

		if(lootParticle)
		{
			particle = UGameplayStatics::SpawnEmitterAttached(lootParticle,particle);
		}
		
		break;
	case E_ItemState::E_PickUp:
		SetActorHiddenInGame(true);
		collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		particle->SetHiddenInGame(true);
		PlayPickupSound();
		break;
	case E_ItemState::E_Equip:
		collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		break;
	default:
		break;
	}
}

void AASItemBase::BounceItem()
{
	FRotator MeshRotation = FRotator(0, itemMeshComponent->GetComponentRotation().Yaw, 0);
	//FRotator MeshRotation = FRotator(0, staticMesh->GetComponentRotation().Yaw, 0);
	//staticMesh
	itemMeshComponent->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	staticMesh->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	float RandomRotation = FMath::RandRange(0, 90);
	float multiply = FMath::RandRange(0,3);

	const FVector MeshRight = itemMeshComponent->GetRightVector();
	//const FVector MeshRight = staticMesh->GetRightVector();

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,FString::Printf(TEXT("%f"),RandomRotation));

	FVector ImpluseDir = MeshRight.RotateAngleAxis(RandomRotation * multiply, FVector(0, 0, 1));
	ImpluseDir *= 1000;

	itemMeshComponent->AddImpulse(ImpluseDir);
	staticMesh->AddImpulse(ImpluseDir);
	bFalling = true;

	GetWorldTimerManager().SetTimer(BounceItemTimer, this, &AASItemBase::StopBounceItem, BounceItemTime);
}

void AASItemBase::StopBounceItem()
{
	bFalling = false;
	SetItemProperties(E_ItemState::E_None);
}

void AASItemBase::PlayPickupSound()
{
	if(PickupSound)
		UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
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
	if(itemDataTable)
	{
		if(itemBaseData.mesh)
		{
			itemMeshComponent->SetSkeletalMesh(itemBaseData.mesh);
		}
		else
		{
			staticMesh->SetStaticMesh(itemBaseData.staticMesh);
		}
	}
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
		if(instance)
		{
			itemDataTable = instance->ItemData;
		}
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

	SetItemProperties(currentState);
	//GetWorld()->GetTimerManager().Tick()
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
	SetSound();

	if(itemBaseData.lootParticle)
		lootParticle = itemBaseData.lootParticle;
}

void AASItemBase::SetSound()
{
	 if(itemBaseData.PickUpSound)
		 PickupSound = itemBaseData.PickUpSound;
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

void AASItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

