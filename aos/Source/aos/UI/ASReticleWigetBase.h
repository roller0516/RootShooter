// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ASReticleWigetBase.generated.h"

/**
 * 
 */

class ASWeapon;
class UImage;
UCLASS(Abstract)
class AOS_API UASReticleWigetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UASReticleWigetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//UFUNCTION(BlueprintImplementableEvent)
	//void OnWeaponInit();
	//
	//UFUNCTION(BlueprintCallable)
	//void InitFromWeapon(ASWeapon* weapon);
	//
	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//float ComputeSpreadAngle() const;
	//
	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//float ComputeMaxScreenspaceSpreadRadius() const;
	//
	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//bool HasFirstShotAccuracy() const;
	//void DrawTexture(UTexture* Texture, float ScreenX, float ScreenY, float ScreenW, float ScreenH, float TextureU, float TextureV, float TextureUWidth, float TextureVHeight, FLinearColor TintColor = FLinearColor::White, EBlendMode BlendMode = BLEND_Translucent, float Scale = 1.f, bool bScalePosition = false, float Rotation = 0.f, FVector2D RotPivot = FVector2D::ZeroVector);
	void DrawReticle();
private:
	//TODO::DATA로 변경 가능하지 않을까
	//	UserWidget ?
	//	크로스 헤어 움직임 ? 뛰거나 했을때 벌어지거나 하는 효과를 위함.
	UPROPERTY(EditAnywhere,meta= (BindWidget))
	TObjectPtr<UImage> UpCrossHair;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> DownCrossHair;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> LeftCrossHair;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> RightCrossHair;

	UPROPERTY(EditAnywhere)
	float CrossHairOffSet;

	UPROPERTY(EditAnywhere)
	float CrossHairWidth;

	UPROPERTY(EditAnywhere)
	float CrossHairHeight;

	UPROPERTY(EditAnywhere)
	float CrossHairSpreadMax;

	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
	FVector2D UpCrossHairValue;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FVector2D BottomCrossHairValue;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FVector2D LeftCrossHairValue;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FVector2D RightCrossHairValue;

	TObjectPtr<class AASPlayerCharacter> PlayerCharacter;

	float CrossHairSpreadMultiplier;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCanvas> Canvas;
protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
