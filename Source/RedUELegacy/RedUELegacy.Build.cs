// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RedUELegacy : ModuleRules
{
	public RedUELegacy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Private/RedUELegacyPCH.h";


		PublicIncludePaths.AddRange(
			new string[] {
				"Public"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"Private"
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
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
                // ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
