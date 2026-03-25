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
				"Core", 
				"MovieScene",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"RenderCore",
				"Projects",
				"LevelSequence",
				"MovieScene",
				"Sequencer",
				"BlueprintGraph",
			}
		);

        PublicIncludePaths.Add(ModuleDirectory);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
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
				"KismetCompiler",
				"AssetDefinition",
				"Kismet",
				"PropertyEditor",
				"MovieSceneTracks",
				"MovieSceneTools",
				"GraphEditor",
				"ToolMenus",
				"WW2OGG",
				"UEOgg",
				"Vorbis",
				"VorbisFile",
				"AudioEditor", 
				"BSPUtils"
			}
		);
	}
}