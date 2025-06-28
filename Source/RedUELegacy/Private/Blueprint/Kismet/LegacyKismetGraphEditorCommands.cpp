#include "LegacyKismetGraphEditorCommands.h"

#define LOCTEXT_NAMESPACE "RbmkEditorModule"
FLegacyKismetGraphEditorCommands::FLegacyKismetGraphEditorCommands():TCommands(TEXT("LegacyKismetGraphEditor"), LOCTEXT( "LegacyKismetGraphEditor", "Legacy Kismet Graph Editor"), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FLegacyKismetGraphEditorCommands::RegisterCommands()
{
	UI_COMMAND( RemoveEventPin, "Remove Event Pin", "Removes the last option event pin from the node", EUserInterfaceActionType::Button, FInputChord() )
}

#undef  LOCTEXT_NAMESPACE 
