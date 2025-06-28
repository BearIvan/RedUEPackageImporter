#include "RedUELegacyModule.h"

#include "ISequencerModule.h"
#include "KismetCompiler.h"
#include "KismetCompilerModule.h"
#include "PropertyEditorModule.h"
#include "Blueprint/Kismet/LegacyKismetBlueprint.h"
#include "Blueprint/Kismet/LegacyKismetCompilerContext.h"
#include "Blueprint/Kismet/LegacyKismetGraphEditorCommands.h"
#include "Blueprint/Kismet/UI/SequenceActionDetails.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Material/Hybrid/HybridMaterialTrackEditor.h"

void FRedUELegacyModule::StartupModule()
{
	{
		FKismetCompilerContext::RegisterCompilerForBP(ULegacyKismetBlueprint::StaticClass(), [](UBlueprint* InBlueprint, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompileOptions)
		{
			return MakeShared<FLegacyKismetCompilerContext>(CastChecked<ULegacyKismetBlueprint>(InBlueprint), InMessageLog, InCompileOptions);
		});
		IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
		KismetCompilerModule.OverrideBPTypeForClass(ALegacyKismet::StaticClass(), ULegacyKismetBlueprint::StaticClass());
		KismetCompilerModule.OverrideBPGCTypeForBPType(ULegacyKismetBlueprint::StaticClass(), ULegacyKismetGeneratedClass::StaticClass());


		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout( UK2Node_SequenceAction::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic( &FSequenceActionDetails::MakeInstance ) );
		
		FLegacyKismetGraphEditorCommands::Register();
		
		ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>( "Sequencer" );
		HybridMaterialTrackEditorHandle  = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FHybridMaterialTrackEditor::CreateTrackEditor));
	}
}

void FRedUELegacyModule::ShutdownModule()
{
	FLegacyKismetGraphEditorCommands::Unregister();
	if (ISequencerModule* SequencerModulePtr = FModuleManager::Get().GetModulePtr<ISequencerModule>("Sequencer"))
	{
		SequencerModulePtr->UnRegisterTrackEditor(HybridMaterialTrackEditorHandle);
	}
}

IMPLEMENT_MODULE(FRedUELegacyModule, RedUELegacy)