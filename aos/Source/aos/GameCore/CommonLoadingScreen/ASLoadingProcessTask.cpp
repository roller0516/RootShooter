// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/CommonLoadingScreen/ASLoadingProcessTask.h"
#include "ASLoadingScreenManager.h"
#include "Engine/GameInstance.h"


UASLoadingProcessTask* UASLoadingProcessTask::CreateProcessTask(UObject* worldContext, const FString& ShowLoadScreenReason)
{
	UWorld* world = GEngine->GetWorldFromContextObject(worldContext,EGetWorldErrorMode::LogAndReturnNull);
	UGameInstance* gameInstance = world->GetGameInstance();
	UASLoadingScreenManager* loadingScreenManager = gameInstance->GetSubsystem<UASLoadingScreenManager>();

	if(loadingScreenManager)
	{
		//loadingScreenManager->register
	}

	return nullptr;
}

bool UASLoadingProcessTask::ShouldShowLoadingScreen(FString& OutReason) const
{
	return true;
}
