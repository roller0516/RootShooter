// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Character/ASBaseCharacter.h"
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
	TArray<TSoftClassPtr<class AASBaseCharacter>> InGameCharacterModule;

	UPROPERTY(EditAnywhere, Category = "UI")
	TArray<TSoftClassPtr<class AASBaseCharacter>> InteractionCharacterModule;

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(FPrimaryAssetType("ASCharacterData"), FName("CharacterData"));
	}


	TSoftClassPtr<class AASBaseCharacter> GetInteractionChar(CharID _charid)
	{
		for(int i = 0 ; i < InteractionCharacterModule.Num(); i++)
		{
			const AASBaseCharacter* base =  Cast<class AASBaseCharacter>(InteractionCharacterModule[i].Get());
			if(base->charID == _charid)
				return InteractionCharacterModule[i];
		}
		return nullptr;
	}
};
