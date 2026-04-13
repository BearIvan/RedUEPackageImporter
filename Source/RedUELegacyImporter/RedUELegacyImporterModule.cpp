#include "RedUELegacyImporterModule.h"

#include "ISequencerModule.h"
#include "ISettingsModule.h"
#include "KismetCompiler.h"
#include "KismetCompilerModule.h"
#include "PropertyEditorModule.h"
#include "Blueprint/Kismet/LegacyKismetCompilerContext.h"
#include "Blueprint/Kismet/LegacyKismetGraphEditorCommands.h"
#include "Blueprint/Kismet/UI/SequenceActionDetails.h"
#include "Core/RedUELegacyImporterSettings.h"
#include "Engine/LevelScriptBlueprint.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Material/Hybrid/HybridMaterialTrackEditor.h"

void FRedUELegacyImporterModule::StartupModule()
{
	{
		FKismetCompilerContext::RegisterCompilerForBP(ULevelScriptBlueprint::StaticClass(), [](UBlueprint* InBlueprint, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompileOptions)
		{
			return MakeShared<FLegacyKismetCompilerContext>(CastChecked<UBlueprint>(InBlueprint), InMessageLog, InCompileOptions);
		});
		
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout( UK2Node_SequenceAction::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic( &FSequenceActionDetails::MakeInstance ) );
		FLegacyKismetGraphEditorCommands::Register();
		
		ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>( "Sequencer" );
		HybridMaterialTrackEditorHandle  = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FHybridMaterialTrackEditor::CreateTrackEditor));
	}
	{
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			auto SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "RedUELegacyImporter",
				NSLOCTEXT("RedUELegacy", "RedUELegacyImporterName", "Red UE Legacy Importer"),
				NSLOCTEXT("RedUELegacy","RedUELegacyImporterNameDescription", ""),
				GetMutableDefault<URedUELegacyImporterSettings>()
			);
		}
	}
}

void FRedUELegacyImporterModule::ShutdownModule()
{
	FLegacyKismetGraphEditorCommands::Unregister();
	if (ISequencerModule* SequencerModulePtr = FModuleManager::Get().GetModulePtr<ISequencerModule>("Sequencer"))
	{
		SequencerModulePtr->UnRegisterTrackEditor(HybridMaterialTrackEditorHandle);
	}
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "RbmkLegacyImporter");
	}
}

IMPLEMENT_MODULE(FRedUELegacyImporterModule, RedUELegacyImporter)