// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/PrimaryAssetId.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/ObjectPtr.h"
#include "ASUserFacingExperienceDefinition.generated.h"

/**
 * 
 */
class UObject;
class UTexture2D;
class UUserWidget;

UCLASS()
class AOS_API UASUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	//맵 로드 ID
	UPROPERTY(BluePrintReadWrite, EditAnyWhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	//유저 메인 위젯이 설정되어있는 Asset 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "ASExperienceDefinition"))
	FPrimaryAssetId UserExperienceID;
	//ui
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience)
	FText Title;

	UPROPERTY(BluePrintReadWrite, EditAnyWhere, Category = Experience)
	FText SubTitle;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience)
	TObjectPtr<UTexture2D> TitleIcon;
	/*
	에셋 매니저와 TSoftObjectPtr, FsoftObjectPtr과 같은 놈들은 디렉토리에 직접적으로 접근하는 시스템이라고 볼 수 있음.
	다른 말로 하면 (만약 해당 데이터가 메모리에 캐싱돼있지 않다면) 런타임에 HDD에 직접적으로 접근한다고 볼 수 있음
	그렇기에 해당 시스템들은 미리 모든 데이터를 메모리에 직접 올려놓지 않아 메모리 사용을 줄이는 장점을 가진다고 할 수도 있지만 반대로 런타임에 
	HDD에 접근하는 고비용 작업을 진행하기에 프레임 드랍의 큰 원인이 될 수 있음.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience)
	TSoftClassPtr<UUserWidget> LoadingScreenWiget;

public:
	
};
