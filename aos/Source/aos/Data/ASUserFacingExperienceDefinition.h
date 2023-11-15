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
	//�� �ε� ID
	UPROPERTY(BluePrintReadWrite, EditAnyWhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	//���� ���� ������ �����Ǿ��ִ� Asset 
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
	���� �Ŵ����� TSoftObjectPtr, FsoftObjectPtr�� ���� ����� ���丮�� ���������� �����ϴ� �ý����̶�� �� �� ����.
	�ٸ� ���� �ϸ� (���� �ش� �����Ͱ� �޸𸮿� ĳ�̵����� �ʴٸ�) ��Ÿ�ӿ� HDD�� ���������� �����Ѵٰ� �� �� ����
	�׷��⿡ �ش� �ý��۵��� �̸� ��� �����͸� �޸𸮿� ���� �÷����� �ʾ� �޸� ����� ���̴� ������ �����ٰ� �� ���� ������ �ݴ�� ��Ÿ�ӿ� 
	HDD�� �����ϴ� ���� �۾��� �����ϱ⿡ ������ ����� ū ������ �� �� ����.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience)
	TSoftClassPtr<UUserWidget> LoadingScreenWiget;

public:
	
};
