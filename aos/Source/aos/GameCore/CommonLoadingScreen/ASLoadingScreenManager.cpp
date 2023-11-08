// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/CommonLoadingScreen/ASLoadingScreenManager.h"
#include "UObject/UObjectGlobals.h"
#include "aos.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ASLoadingScreenManager) // 헤더 인라인 컴파일 시간 단축

void UASLoadingScreenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this,&ThisClass::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&ThisClass::HandlePostLoadMap);

	const UGameInstance* LocalGameInstance = GetGameInstance();
	check(LocalGameInstance);
}

void UASLoadingScreenManager::Deinitialize()
{

}

bool UASLoadingScreenManager::ShouldCreateSubsystem(UObject* Outer) const
{
	//const UGameInstance* GameInstance = CastChecked<UGameInstance>(Outer);
	//const bool bIsServerWorld = GameInstance->IsDedicatedServerInstance();
	return true;
}

void UASLoadingScreenManager::HandlePreLoadMap(const FWorldContext& WorldContext, const FString& MapName)
{
	const UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		if (UGameViewportClient* ViewportClient = GameInstance->GetGameViewportClient())
		{
			UE_LOG(LogLoadScreenManager, Log, TEXT("%s"), *MapName);
		}
	}
}

void UASLoadingScreenManager::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if(LoadedWorld && LoadedWorld->GetGameInstance())
	{
		UE_LOG(LogLoadScreenManager,Log,TEXT("%s"),*LoadedWorld->GetName());
	}
}

void UASLoadingScreenManager::Tick(float DeltaTime)
{
	
}

TStatId UASLoadingScreenManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UASLoadingScreenManager, STATGROUP_Tickables);
}
