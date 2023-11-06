// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class aos : ModuleRules
{
	public aos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });

        PrivateIncludePaths.Add("aos");

	}
}
