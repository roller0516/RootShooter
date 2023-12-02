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

	FName BarrelSocketName;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Wepon Properties",meta = (AllowPrivateAccess = "true"))
	bool bMovingClip;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UNiagaraSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UNiagaraSystem> ShellEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UNiagaraSystem> TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UStaticMesh> ShellMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float MinDamage = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float MaxDamage = 12;

public:
	void ShowShotParticles(FHitResult pHitResult);
	FTransform GetBarrelSocketTransForm() const;

	FORCEINLINE FName GetClipBoneName() const {return ClipBoneName;}
	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }
	FORCEINLINE float GetDamage();
	FORCEINLINE float GetHeadDamage();

};
