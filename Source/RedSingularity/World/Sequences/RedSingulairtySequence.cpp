#include "RedSingulairtySequence.h"

#include "EdGraphSchema_K2.h"
#include "K2Node_CustomEvent.h"

UK2Node* ULegacyRSeqEvt_CheckpointActivated::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	UK2Node_CustomEvent* NewEventNode = NewObject<UK2Node_CustomEvent>(InGraph);
	NewEventNode->CustomFunctionName = GetLegacyFName();
	NewEventNode->CreateNewGuid();
	NewEventNode->PostPlacedNewNode();
	NewEventNode->SetFlags(RF_Transactional);
	NewEventNode->AllocateDefaultPins();
	NewEventNode->bCommentBubblePinned = true;
	NewEventNode->NodePosY = 0;
	NewEventNode->OnUpdateCommentText(GetLegacyFullName());
	UEdGraphSchema_K2::SetNodeMetaData(NewEventNode, FNodeMetadata::DefaultGraphNode);
	InGraph->AddNode(NewEventNode);
	if(ensure(OutputLinks.Num()==1))
	{
		check(OutputLinks[0].Links.Num() == 1);
		if(OutputLinks[0].Links[0].LinkedOp)
		{
			
			if(UEdGraphPin*InCurrentPinNode = CastChecked<ULegacySequenceAction>(OutputLinks[0].Links[0].LinkedOp)->GetPinOrCreateNode(InBlueprint,InGraph,OutputLinks[0].Links[0].InputLinkIdx))
			{
				NewEventNode ->GetThenPin()->MakeLinkTo(InCurrentPinNode);
			}
		}
	}
	CurrentNode = NewEventNode;
	return NewEventNode;
}
