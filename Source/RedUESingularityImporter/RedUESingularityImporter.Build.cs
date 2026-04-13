using UnrealBuildTool;

public class RedUESingularityImporter : ModuleRules
{
    public RedUESingularityImporter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "RedUELegacyImporter",
                "RedUEBioshock3",
                "BlueprintGraph",
                "MovieScene",
                "UnrealEd",
                "Sequencer",
                "MovieSceneTracks",
            }
        );

    }
}