// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASItemBase.h"
#include "ASAmmo.generated.h"

UCLASS()
class AOS_API AASAmmo : public AASItemBase
{
	GENERATED_BODY()
	
public:	
	AASAmmo();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UseItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
