// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASGameInstance.h"


UASGameInstance::UASGameInstance(const FObjectInitializer& ObjectInitializer) 
:Super(ObjectInitializer)
{

}

void UASGameInstance::Init()
{
	Super::Init();

	//UASLoadingScreenManager*  GetSubsystem<UASLoadingScreenManager>();
}

void UASGameInstance::Shutdown()
{
	Super::Shutdown();
}
