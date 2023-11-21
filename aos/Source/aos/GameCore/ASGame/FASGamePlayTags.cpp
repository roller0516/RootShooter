#include "FASGamePlayTags.h"
#include "GameplayTagsManager.h"

FASGameplayTags FASGameplayTags::GameplayTags;
void FASGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Tag_UI_Layer_Menu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("UI.Layer.Menu"),
		FString("UI Layer Menu")
	);


	GameplayTags.Tag_UI_Action_Escape = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("UI.Action.Escape"),
		FString("UI Action Escape")
	);
}

