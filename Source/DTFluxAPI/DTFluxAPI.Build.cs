// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DTFluxAPI : ModuleRules
{
	public DTFluxAPI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
			);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"HTTPServer",
				"HTTP",
				"DeveloperToolSettings",
				"DeveloperSettings",
				"Json", 
				"JsonUtilities",
				"AvalancheCore",
				"AvalancheMedia",
			}
			);
	}
}
