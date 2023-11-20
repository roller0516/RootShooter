// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interface/Command/ILoadingProcessInterFace.h"

#include "ASLoadingProcessTask.generated.h"

/**
 * 
 */

//�ε� ��ũ���� ���� ������ CreateProcessTask�� ����ϸ��.
//�ᱹ ASUILoadingScreen���� �����Ͽ� �����ϰԵ�.	

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
