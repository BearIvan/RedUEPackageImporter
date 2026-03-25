using UnrealBuildTool;

public class RedUEBioshock3Importer : ModuleRules
{
    public RedUEBioshock3Importer(ReadOnlyTargetRules Target) : base(Target)
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