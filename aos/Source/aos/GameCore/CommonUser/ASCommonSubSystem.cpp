// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/CommonUser/ASCommonSubSystem.h"

UE_DEFINE_GAMEPLAY_TAG(FCommonUserTags::SystemMessage_Error, "SystemMessage.Error");

void UASCommonSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UASCommonSubSystem::Deinitialize()
{
	
}
