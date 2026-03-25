// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RedUELegacyImporter : ModuleRules
{
	public RedUELegacyImporter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "RedUELegacyImporterPCH.h";

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "MovieScene",
			}
		);

        PublicIncludePaths.Add(ModuleDirectory);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"RenderCore",
				"Projects",
				"UnrealEd",
				"InputCore",
				"MeshDescription",
				"StaticMeshDescription",
				"MaterialEditor",
				"MaterialUtilities",
				"zlib",
				"LZO",
				"LZX",
				"RedImage",
				"EditorSubsystem",
				"RedUELegacyRuntime",
				"BlueprintGraph",
				"KismetCompiler",
				"AssetDefinition",
				"Kismet",
				"PropertyEditor",
				"LevelSequence",
				"MovieScene",
				"Sequencer",
				"MovieSceneTracks",
				"MovieSceneTools",
				"GraphEditor",
				"ToolMenus",
				"WW2OGG",
				"UEOgg",
				"Vorbis",
				"VorbisFile",
				"AudioEditor", "BSPUtils"
			}
		);
	}
}