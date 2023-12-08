// Copyright longlt00502@gmail.com 2023. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

DEFINE_LOG_CATEGORY_STATIC(EasyPose, Log, All);

enum class MatchMode : uint8 {
	SIMILAR, DYNAMIC, EXACT
};

namespace matching {
enum class EStep : uint8 {
	MatchMiddleBoneRotation, MatchBoneTailRotation, MatchSubsequenceChain, Max
};

enum class EFlag : uint32 {
	StepMatchMiddleBoneRotation = 1u << static_cast<uint8>(EStep::MatchMiddleBoneRotation),
	StepMatchBoneTailRotation = 1u << static_cast<uint8>(EStep::MatchBoneTailRotation),
	StepMatchSubsequenceChain = 1u << static_cast<uint8>(EStep::MatchSubsequenceChain),
	MatchAll = (StepMatchMiddleBoneRotation | StepMatchBoneTailRotation | StepMatchSubsequenceChain)
};

}

class /*EASYPOSE_API*/ FEasyPoseModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void PluginButtonClicked();
	void OnPostEngineInit();
	
private:

	int MatchPose(class UIKRetargeter *iKRetargeter);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TArray<TSharedPtr<FString>> FPToleranceOptions = { MakeShared<FString>(TEXT("0.01 (1e-2)")), MakeShared<FString>(TEXT("0.0001 (1e-4)")), MakeShared<FString>(TEXT("0.00000001 (1e-8)")) };
	TSharedPtr<FString> FPTSelected = FPToleranceOptions[1];
	float FPTolerance = 1e-4;

	float BoneAxesDotProductScalar = 0.8f;
	int32 RotationOffsetFloatingPointNegativeExp = 4;
	uint32 StepFlag = static_cast<uint32>(matching::EFlag::MatchAll);
	bool AlwaysReset = true;
	bool StepMiddleBoneXOnly = true;
	MatchMode MatchingMode = MatchMode::DYNAMIC;

	inline bool ShouldMatch(matching::EFlag Flag)
	{
		return (StepFlag & static_cast<uint32>(Flag)) != 0;
	}

};
