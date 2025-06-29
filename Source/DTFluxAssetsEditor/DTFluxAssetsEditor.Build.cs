using UnrealBuildTool;

public class DTFluxAssetsEditor : ModuleRules
{
    public DTFluxAssetsEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "AssetTools",
                "SlateCore",
                "UnrealEd",
                "DTFluxCore",
            }
        );
    }
}