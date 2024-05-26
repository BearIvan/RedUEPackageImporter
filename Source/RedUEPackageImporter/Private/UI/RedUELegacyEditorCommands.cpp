#include "RedUELegacyEditorCommands.h"

#define LOCTEXT_NAMESPACE "RedUEPackageImporterModule"
FRedUELegacyEditorCommands::FRedUELegacyEditorCommands(): TCommands<FRedUELegacyEditorCommands>(TEXT("RedUELegacyEditorCommands"), NSLOCTEXT("Contexts", "RedUELegacyEditorCommands", "UE Legacy Editor Commands"), NAME_None, NAME_None)
{
}

void FRedUELegacyEditorCommands::RegisterCommands()
{
	UI_COMMAND(ShowUELegacyExplorer, "ShowUELegacyExplorer", "Show UE Legacy Explorer", EUserInterfaceActionType::Button, FInputChord())
}
#undef LOCTEXT_NAMESPACE