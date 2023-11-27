// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ASCharacterData.generated.h"

/**
 * 
 */
UENUM()
enum class ShootParticle : uint8
{
	Muzzle,	
};

UCLASS()
class AOS_API UASCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Name;
	
	UPROPERTY(EditAnywhere)
	class USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere)
	class UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere)
	TMap<ShootParticle,class UParticleSystem*> particles;

	UPROPERTY(EditAnywhere,Category = "Battle")
	TArray<TSubclassOf<class AASPlayerCharacter>> InGameCharacterBp;

	UPROPERTY(EditAnywhere,Category = "UI")
	TArray<TSubclassOf<class AASPlayerCharacter>> InGameCharacterBp;

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(FPrimaryAssetType("ASCharacterData"),"CharacterData");
	}
};
