// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class aos : ModuleRules
{
	public aos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" , "GameplayTags", "UMG" , "Slate" , "SlateCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Niagara", "DeveloperSettings", "RenderCore" , "CommonInput" , "ModularGameplay" ,"GameFeatures", "ControlFlows" });

        PrivateIncludePaths.Add("aos");
		
	}
}
