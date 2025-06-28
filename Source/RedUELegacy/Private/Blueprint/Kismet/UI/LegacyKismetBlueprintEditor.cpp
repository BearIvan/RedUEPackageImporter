#include "LegacyKismetBlueprintEditor.h"

#include "EdGraphSchema_K2.h"
#include "LegacyKismetNodeBindingExtension.h"
#include "Blueprint/Kismet/EdGraphSchema_LegacyKismet.h"
#include "Blueprint/Kismet/K2Node_SequenceAction.h"
#include "Blueprint/Kismet/LegacyKismetGraphEditorCommands.h"
#include "Widgets/Docking/SDockTab.h"

void FLegacyKismetBlueprintEditor::InitKismetBlueprintEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, const TArray<class UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
{
	InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);

	Inspector->GetPropertyView()->SetExtensionHandler(MakeShared<FLegacyKismetNodeBindingExtension>());
}

void FLegacyKismetBlueprintEditor::OnRemoveEventPin()
{
	TSharedPtr<SDockTab> ActiveTab = DocumentManager->GetActiveTab();
	if (ActiveTab.IsValid())
	{
		TSharedPtr<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(ActiveTab->GetContent());
		if (GraphEditor.IsValid())
		{
			UEdGraphPin* SelectedPin = GraphEditor->GetGraphPinForMenu();
			if (ensure(SelectedPin))
			{
				UK2Node_SequenceAction* SelectedNode = Cast<UK2Node_SequenceAction>(SelectedPin->GetOwningNode());

				if (SelectedNode && SelectedNode == SelectedPin->GetOwningNode())
				{
					SelectedNode->RemoveEventPin(SelectedPin);
				}
			}
		}
	}
}

bool FLegacyKismetBlueprintEditor::CanAddRemovePin()
{
	TSharedPtr<SDockTab> ActiveTab = DocumentManager->GetActiveTab();
	if (ActiveTab.IsValid())
	{
		TSharedPtr<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(ActiveTab->GetContent());
		if (GraphEditor.IsValid())
		{
			UEdGraphPin* SelectedPin = GraphEditor->GetGraphPinForMenu();
			if (ensure(SelectedPin))
			{
				UK2Node_SequenceAction* SelectedNode = Cast<UK2Node_SequenceAction>(SelectedPin->GetOwningNode());

				if (SelectedNode && SelectedNode == SelectedPin->GetOwningNode())
				{
					return SelectedPin->Direction == EGPD_Input && SelectedPin->PinType.PinCategory  == UEdGraphSchema_K2::PC_Delegate;
				}
			}
		}
	}
	return false;
}

void FLegacyKismetBlueprintEditor::OnCreateGraphEditorCommands(TSharedPtr<FUICommandList> GraphEditorCommandsList)
{
	GraphEditorCommandsList->MapAction(FLegacyKismetGraphEditorCommands::Get().RemoveEventPin, FExecuteAction::CreateSP(this, &FLegacyKismetBlueprintEditor::OnRemoveEventPin), FCanExecuteAction::CreateSP(this, &FLegacyKismetBlueprintEditor::CanAddRemovePin));
}

TSubclassOf<UEdGraphSchema> FLegacyKismetBlueprintEditor::GetDefaultSchemaClass() const
{
	return UEdGraphSchema_LegacyKismet::StaticClass();
}