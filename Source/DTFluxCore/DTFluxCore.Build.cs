using UnrealBuildTool;

public class DTFluxCore : ModuleRules
{
    public DTFluxCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core","JsonUtilities"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DTFluxProjectSettings",
                "JsonUtilities",
                "Json", 

            }
        );
    }
}