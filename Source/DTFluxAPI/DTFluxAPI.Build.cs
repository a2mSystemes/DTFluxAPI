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
				"Core", "AvalancheText", 
			}
			);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"EditorSubsystem",
				"EditorFramework",
				"Slate",
				"SlateCore",
				"HTTPServer",
				"HTTP",
				"Networking",
				"WebSockets",
				"WebSocketNetworking",
				"DeveloperToolSettings",
				"DeveloperSettings",
				"Json", 
				"JsonUtilities",
				"SlateCore",
				"Text3D",
				"AvalancheCore",
				"AvalancheMedia",
				"AvalancheText", 
			}
			);
	}
}
