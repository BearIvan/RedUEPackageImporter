// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RedUEPackageImporter : ModuleRules
{
	public RedUEPackageImporter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Private/RedUEPackageImporterPCH.h";


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
				"RedUELegacy",
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
                "EditorStyle",
                "RenderCore",
                "ToolMenus",
                "WorkspaceMenuStructure",
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
