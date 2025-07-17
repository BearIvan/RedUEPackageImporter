// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RedUELegacy : ModuleRules
{
	public RedUELegacy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Private/RedUELegacyPCH.h";

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "MovieScene",
			}
		);


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
				"RedUELegacyGame",
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
				"ToolMenus"
			}
		);
	}
}