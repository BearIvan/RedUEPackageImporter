using UnrealBuildTool;

public class RedUELegacyGame : ModuleRules
{
    public RedUELegacyGame(ReadOnlyTargetRules Target) : base(Target)
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
                "Projects",
                "UnrealEd",
                "RenderCore",
            }
        );
        
        PublicIncludePaths.Add(ModuleDirectory);
    }
}