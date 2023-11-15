// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/CommonGame/ASGameUIManagerSubSystem.h"
#include "UI/UICore/ASGameUIPolicy.h"


void UASGameUIManagerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(!currentPolicy && !defualtPolicyClass.IsNull())
	{
		TSubclassOf<UASGameUIPolicy> policyClass = defualtPolicyClass.LoadSynchronous();
		ChangeUIPolicy(NewObject<UASGameUIPolicy>(this,policyClass));
	}
}

void UASGameUIManagerSubSystem::Deinitialize()
{
	Super::Deinitialize();

	ChangeUIPolicy(nullptr);
}

void UASGameUIManagerSubSystem::NotifyAddPlayer(UASCommonLocalPlayer* localPlayer)
{
	if(ensure(localPlayer) && currentPolicy)
	{
		currentPolicy->NotifyAddPlayer(localPlayer);
	}
}

void UASGameUIManagerSubSystem::NotifyRemovePlayer(UASCommonLocalPlayer* localPlayer)
{
	if (localPlayer && currentPolicy)
	{
		currentPolicy->NotifyRemovePlayer(localPlayer);
	}
}

void UASGameUIManagerSubSystem::NotifyDestroyPlayer(UASCommonLocalPlayer* localPlayer)
{
	if (localPlayer && currentPolicy)
	{
		currentPolicy->NotifyDestroyPlayer(localPlayer);
	}
}

void UASGameUIManagerSubSystem::ChangeUIPolicy(UASGameUIPolicy* policy)
{
	if (currentPolicy != policy)
	{
		currentPolicy = policy;
	}
}

//bool UASGameUIManagerSubSystem::ShouldCreateSubsystem(UObject* Outer) const
//{
//	return true;
//}
