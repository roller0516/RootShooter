// Copyright longlt00502@gmail.com 2023. All rights reserved.

using UnrealBuildTool;

public class EasyPose : ModuleRules
{
	public EasyPose(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				System.IO.Path.Combine(GetModuleDirectory("Persona"), "Private"),
				// System.IO.Path.Combine(GetModuleDirectory("VersatileEquipment"), "Public"),
			}
			);
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"EditorSubsystem",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore",
				"Persona",

				"Core",
				"ToolWidgets",
				"MessageLog",
				"IKRig",
				"IKRigEditor",
				// ... add private dependencies that you statically link with here ...	
			}
			);
	}
}
