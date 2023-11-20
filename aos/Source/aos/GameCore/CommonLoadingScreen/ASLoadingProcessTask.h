// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interface/Command/ILoadingProcessInterFace.h"

#include "ASLoadingProcessTask.generated.h"

/**
 * 
 */

//로딩 스크린을 띄우고 싶을때 CreateProcessTask를 사용하면됨.
//결국 ASUILoadingScreen에서 수집하여 관리하게됨.	

UCLASS()
class AOS_API UASLoadingProcessTask : public UObject ,public IILoadingProcessInterFace
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable)
	static UASLoadingProcessTask* CreateProcessTask(UObject* worldContext,const FString& ShowLoadScreenReason);
public:
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;

	void SetShowLoadingScreenReason(const FString& InReason);
	void Unregister();
private:
	FString Reason;
};
