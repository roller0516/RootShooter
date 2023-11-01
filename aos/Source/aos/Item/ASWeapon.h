// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASItemBase.h"
#include "ASWeapon.generated.h"

/**
 * 
 */
UCLASS()
class AOS_API AASWeapon : public AASItemBase
{
	GENERATED_BODY()
public:
	AASWeapon();

private:
	UPROPERTY(EditAnywhere, Category = "Wepon Properties", meta=(AllowPrivateAccess = "true"))
	FName ClipBoneName;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Wepon Properties",meta = (AllowPrivateAccess = "true"))
	bool bMovingClip;
public:
	FORCEINLINE FName GetClipBoneName() const {return ClipBoneName;}
	FORCEINLINE void SetMovingClip(bool Move) {bMovingClip = Move;}
};
