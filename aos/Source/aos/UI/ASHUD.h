// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ASHUD.generated.h"

/**
 * 
 */
UCLASS()
class AOS_API AASHUD : public AHUD
{
	GENERATED_BODY()
public:
	AASHUD(const FObjectInitializer& OjbectInit);

	//virtual void DrawHUD() override;
	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
//private:
	// TODO::DATA로 변경 가능하지 않을까
	// UserWidget?
	// 크로스 헤어 움직임? 뛰거나 했을때 벌어지거나 하는 효과를 위함.
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UTexture> UpCrossHair;
	//
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UTexture> DownCrossHair;
	//
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UTexture> LeftCrossHair;
	//
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UTexture> RightCrossHair;
	//
	//UPROPERTY(EditAnywhere)
	//float CrossHairOffSet;
	//
	//UPROPERTY(EditAnywhere)
	//float CrossHairWidth;
	//
	//UPROPERTY(EditAnywhere)
	//float CrossHairHeight;
	//
	//UPROPERTY(EditAnywhere)
	//float CrossHairSpreadMax;
	//
	//TObjectPtr<class AASPlayerCharacter> PlayerCharacter;
	//
	//float CrossHairSpreadMultiplier;
};
