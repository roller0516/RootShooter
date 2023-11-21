#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FASGameplayTags
{
public:
	static const FASGameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag Tag_UI_Layer_Menu;

	FGameplayTag Tag_UI_Action_Escape;

private:
	static FASGameplayTags GameplayTags;
};