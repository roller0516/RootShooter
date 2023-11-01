// Fill out your copyright notice in the Description page of Project Settings.


#include "ASItemBase.h"

#include "Components/BoxComponent.h"

// Sets default values
AASItemBase::AASItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ItemMesh);
}

// Called when the game starts or when spawned
void AASItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

