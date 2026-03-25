#include "SGraphNodeK2SequenceAction.h"
#include "GraphEditorSettings.h"
#include "ScopedTransaction.h"
#include "Blueprint/Kismet/K2Node_SequenceAction.h"

#define LOCTEXT_NAMESPACE "RedUELegacy"
void SGraphNodeK2SequenceAction::Construct(const FArguments& InArgs, class UK2Node_SequenceAction* InNode)
{
	this->GraphNode = InNode;
	this->SequenceNode = InNode;

	this->SetCursor(EMouseCursor::CardinalCross);

	this->UpdateGraphNode();
}

void SGraphNodeK2SequenceAction::CreateInputSideAddButton(TSharedPtr<SVerticalBox> OutputBox)
{
	TSharedRef<SWidget> AddPinButton = AddPinButtonContent(LOCTEXT("AddInput","Add input"),LOCTEXT("AddInput_Tooltip","Adds an input to the dialog node"));

	FMargin AddPinPadding = Settings->GetInputPinPadding();
	AddPinPadding.Top += 6.0f;

	OutputBox->AddSlot()
	.AutoHeight()
	.VAlign(VAlign_Center)
	.Padding(AddPinPadding)
	[
		AddPinButton
	];
}

EVisibility SGraphNodeK2SequenceAction::IsAddPinButtonVisible() const
{
	EVisibility ButtonVisibility = SGraphNode::IsAddPinButtonVisible();
	if (ButtonVisibility == EVisibility::Visible)
	{
		if (!SequenceNode->CanAddEventPin())
		{
			ButtonVisibility = EVisibility::Collapsed;
		}
	}
	return ButtonVisibility;
}

FReply SGraphNodeK2SequenceAction::OnAddPin()
{
	const FScopedTransaction Transaction( NSLOCTEXT("RedUELegacy","AddSequenceActionEvent_Transaction","Add Event Input") );
	SequenceNode->AddEventPin();

	return FReply::Handled();
}
#undef LOCTEXT_NAMESPACE