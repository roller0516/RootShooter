#include "FASGamePlayTags.h"
#include "GameplayTagsManager.h"

FASGameplayTags FASGameplayTags::GameplayTags;
void FASGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Tag_UI_Layer_Menu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("UI.Layer.Menu"),
		FString("UI Layer Menu")
	);
}

