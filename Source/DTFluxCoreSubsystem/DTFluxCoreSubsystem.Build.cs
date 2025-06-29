using UnrealBuildTool;

public class DTFluxCoreSubsystem : ModuleRules
{
    public DTFluxCoreSubsystem(ReadOnlyTargetRules Target) : base(Target)
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
                "DTFluxNetwork",
                "DTFluxProjectSettings",
                "DTFluxCore",  
                "JsonUtilities",
                "Json"
            }
        );
    }
}