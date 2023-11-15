// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ASGameUIManagerSubSystem.generated.h"

/**
 * Game UI Manager 레이아웃을 변경하거나 UIPolicy 관리하는 Class 
 */

class UASGameUIPolicy;
class UASCommonLocalPlayer;
UCLASS(config = Game)
class AOS_API UASGameUIManagerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UASGameUIManagerSubSystem(){}

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//virtual bool ShouldCreateSubsystem(UObject* Outer)const override;

	UASGameUIPolicy* GetCurrnetGameUIPolicy() const {return currentPolicy;}
	UASGameUIPolicy* GetCurrnetGameUIPolicy() {return currentPolicy;}

	virtual void NotifyAddPlayer(UASCommonLocalPlayer* localPlayer);
	virtual void NotifyRemovePlayer(UASCommonLocalPlayer* localPlayer);
	virtual void NotifyDestroyPlayer(UASCommonLocalPlayer* localPlayer);
protected:
	void ChangeUIPolicy(UASGameUIPolicy* policy);
private:

	UPROPERTY()
	TObjectPtr<UASGameUIPolicy> currentPolicy;

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UASGameUIPolicy> defualtPolicyClass;

	
};
