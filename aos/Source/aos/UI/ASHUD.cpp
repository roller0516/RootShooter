// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASHUD.h"

#include "Character/ASPlayerCharacter.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "Components/GameFrameworkComponentManager.h"

//AASHUD::AASHUD():
//CrossHairOffSet(50.f),
//CrossHairHeight(32.f),
//CrossHairWidth(32.f),
//CrossHairSpreadMax(16.f)
//{
//	PrimaryActorTick.bCanEverTick = true;
//}

AASHUD::AASHUD(const FObjectInitializer& OjbectInit)
:Super(OjbectInit)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
}



void AASHUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::BeginPlay();
	//if(!PlayerCharacter.Get()->IsValidLowLevel())
	//{
	//	PlayerCharacter = Cast<AASPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	//}
}

void AASHUD::PreInitializeComponents()
{
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AASHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	
	Super::EndPlay(EndPlayReason);
	//throw std::logic_error("The method or operation is not implemented.");
}

