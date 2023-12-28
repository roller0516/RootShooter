// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/Command/ILoadingProcessInterFace.h"
#include "Components/GameStateComponent.h"
#include "ControlFlowNode.h"
#include "../../EnumTypes.h"

#include "ASCharacterSelectComponent.generated.h"

DECLARE_DELEGATE_OneParam(FCharacterSelectComplete,bool);

class FControlFlow;
class UCommonActivatableWidget;
class UASExperienceDefinition;

UCLASS()
class AOS_API UASCharacterSelectComponent : public UGameStateComponent, public IILoadingProcessInterFace
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UASCharacterSelectComponent(const FObjectInitializer& ObjectInitializer);
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	virtual	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void OnExperienceLoadComplete(const UASExperienceDefinition* experience);

	void FlowStep_WaitForUserInitialization(FControlFlowNodeRef SubFlow);
	void FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow);
	void FlowStep_WaitForCharacterSpawn(FControlFlowNodeRef SubFlow);

	void LoadSelectCharacters(TFunction<void(bool)> result);

	//UFUNCTION(BlueprintCallable)
	//void ZoomInCharacter();
	//UFUNCTION(BlueprintCallable)
	//void ZoomOutCharacter();

	bool bShouldShowLoadingScreen;

private:
	FCharacterSelectComplete OnCharacterSelectComplete;

	TSharedPtr<FControlFlow> FrontEndFlow;

	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> mainScreenClass;

	UPROPERTY(EditAnywhere,Category = "SpawnTransForm")
	TArray<TSubclassOf<AActor>> SpawnTransForm;

	UPROPERTY();
	TObjectPtr<class UASCharacterData> charData;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TSoftClassPtr<class AASBaseCharacter>> LoadCharList;

	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AActor>> cameraList;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AActor> mainCam;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AASBaseCharacter>> AfterLoadCharList;

	UASExperienceDefinition* experience;
};
