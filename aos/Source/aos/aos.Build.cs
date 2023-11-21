// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class aos : ModuleRules
{
	public aos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" , "GameplayTags", "UMG" , "Slate" , "SlateCore" , "ModularGameplay", "GameFeatures", "CommonUI"});

        PrivateDependencyModuleNames.AddRange(new string[] { "Niagara", "DeveloperSettings", "RenderCore" , "CommonInput" ,  "ControlFlows" });

        PrivateIncludePaths.Add("aos");
		
	}
}
