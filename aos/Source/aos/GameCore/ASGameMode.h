// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASGameMode.generated.h"

/*
 *	플레이중인 게임의 정보를 처리하는 클래스

	- 존재하는 플레이어와 관람자의 수는 물론, 허용된 플레이어와 관람자 최대 수
	- 플레이어가 게임에 들어오는 방식, 스폰 위치 선택 규칙과 기타 스폰/리스폰 동작 포함 가능
	- 게임 일시정지 가능 여부, 게임 일시정지 처리 방식
	- 레벨간의 전환, 게임의 시네마틱 모드 시작 여부 포함 
 */
enum class LevelState : uint8
{
	Title,
	Game,
	Dungeon
};


class UASUserFacingExperienceDefinition;

UCLASS()
class AOS_API AASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	FORCEINLINE void SetCurrentLevelState(LevelState levelState) {currentLevelState = levelState;}

private:
	UPROPERTY(EditAnywhere)
	TArray<UASUserFacingExperienceDefinition*> MapData;

	//UPROPERTY(VisibleAnywhere)
	LevelState currentLevelState = LevelState::Title;
};
