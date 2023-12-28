// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "ControlFlowNode.h"
#include "Interface/Command/ILoadingProcessInterFace.h"
#include "ASFrontendComponent.generated.h"

class FControlFlow;
class UCommonActivatableWidget;
class UASExperienceDefinition;


/**
 * 
 */
UCLASS()
class AOS_API UASFrontendComponent : public UGameStateComponent ,public IILoadingProcessInterFace
{
	GENERATED_BODY()
public:

	UASFrontendComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool ShouldShowLoadingScreen(FString& OutReason)const override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	void OnExperiencedLoaded(const UASExperienceDefinition* Experience);

	void FlowStep_WaitForUserInitialization(FControlFlowNodeRef SubFlow);
	void FlowStep_TryShowPressStartScreen(FControlFlowNodeRef SubFlow);
	void FlowStep_TryJoinRequestedSession(FControlFlowNodeRef SubFlow);
	void FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow);
private:
	bool bShouldShowLoadingScreen;
	
	TSharedPtr<FControlFlow> FrontEndFlow;

	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> mainScreenClass;

	UASExperienceDefinition* experience;
};
