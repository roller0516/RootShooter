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
		UASLoadingProcessTask* NewLoadingTask = NewObject<UASLoadingProcessTask>(loadingScreenManager);
		NewLoadingTask->SetShowLoadingScreenReason(ShowLoadScreenReason);

		loadingScreenManager->RegisterLoadingProcessInterFace(NewLoadingTask);

		return NewLoadingTask;
	}

	return nullptr;
}

void UASLoadingProcessTask::Unregister()
{
	UASLoadingScreenManager* LoadingScreenManager = Cast<UASLoadingScreenManager>(GetOuter());
	LoadingScreenManager->UnRegisterLoadingProcessInterFace(this);
}

void UASLoadingProcessTask::SetShowLoadingScreenReason(const FString& InReason)
{
	Reason = InReason;
}

bool UASLoadingProcessTask::ShouldShowLoadingScreen(FString& OutReason) const
{
	OutReason = Reason;
	return true;
}
