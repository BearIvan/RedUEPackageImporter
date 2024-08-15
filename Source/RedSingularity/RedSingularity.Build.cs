using UnrealBuildTool;

public class RedSingularity : ModuleRules
{
    public RedSingularity(ReadOnlyTargetRules Target) : base(Target)
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
                "RedUELegacy",
                "BlueprintGraph"
            }
        );
        
        PrivatePCHHeaderFile = "RedSingularityPCH.h";
    }
}