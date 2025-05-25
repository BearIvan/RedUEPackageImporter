#include "RedUELegacyModule.h"

#include "KismetCompiler.h"
#include "KismetCompilerModule.h"
#include "PropertyEditorModule.h"
#include "Blueprint/Kismet/LegacyKismetBlueprint.h"
#include "Blueprint/Kismet/LegacyKismetCompilerContext.h"
#include "Blueprint/Kismet/UI/SequenceActionDetails.h"
#include "Kismet/Base/LegacyKismet.h"

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
		
	}
}

void FRedUELegacyModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FRedUELegacyModule, RedUELegacy)