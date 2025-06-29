using UnrealBuildTool;

public class DTFluxNetwork : ModuleRules
{
    public DTFluxNetwork(ReadOnlyTargetRules Target) : base(Target)
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
                "SlateCore",
                "WebSockets",
                "HTTP",
                "DTFluxCore",
                "DTFluxProjectSettings",
                "JsonUtilities",
                "Json",
            }
        );
    }
}