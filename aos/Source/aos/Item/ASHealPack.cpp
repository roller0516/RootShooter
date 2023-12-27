// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ASHealPack.h"
#include "Character/ASPlayerCharacter.h"

// Sets default values
AASHealPack::AASHealPack()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthBonus = FMath::RandRange(MinHeal, MaxHeal);
}

// Called when the game starts or when spawned
void AASHealPack::BeginPlay()
{
	Super::BeginPlay();
}

void AASHealPack::UseItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto playerCharacter = Cast<AASPlayerCharacter>(OtherActor);
	if (playerCharacter)
	{
		playerCharacter->Heal(HealthBonus);
		Destroy();
	}
}
