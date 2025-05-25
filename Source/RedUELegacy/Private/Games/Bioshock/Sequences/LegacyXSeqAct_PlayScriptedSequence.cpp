#include "LegacyXSeqAct_PlayScriptedSequence.h"
#include "K2Node_CallFunction.h"
#include "Blueprints/RedUEBlueprintFunctionLibrary.h"
//
// UEdGraphPin* ULegacyXSeqAct_PlayScriptedSequence::GetPinOrCreateNode(UBlueprint* InBlueprint, UEdGraph* InGraph, int32 InputLinkIdx)
// {
// 	if (!ensure(VariableLinks.Num() == 4&&InputLinks.Num() == 2&&OutputLinks.Num() == 2))
// 	{
// 		return nullptr;
// 	}
// 	if(!ensure(InputLinkIdx<2))
// 	{
// 		return nullptr;
// 	}
// 	static FName NAME_Ready = "Ready";
// 	static FName NAME_CleanedUp = "CleanedUp";
// 	if(!CurrentNode)
// 	{
// 		FGraphNodeCreator<UK2Node_CallFunction> NodeCreator(*InGraph);
// 		UK2Node_CallFunction* NewNode = NodeCreator.CreateNode();
// 		NewNode->SetFromFunction(URedUEBlueprintFunctionLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(URedUEBlueprintFunctionLibrary, XPlayScriptedSequence)));
// 		NodeCreator.Finalize();
// 		CurrentNode = NewNode;
//
// 		if (OutputLinks[0].Links.Num() > 0&& OutputLinks[0].Links[0].LinkedOp)
// 		{
// 			if(UEdGraphPin*InCurrentPinNode = CastChecked<ULegacySequenceAction>(OutputLinks[0].Links[0].LinkedOp)->GetPinOrCreateNode(InBlueprint,InGraph,OutputLinks[0].Links[0].InputLinkIdx))
// 			{
// 				UEdGraphPin* InputPin = CurrentNode ->FindPin(NAME_Ready);
// 				if (ensure(InputPin))
// 				{
// 					InputPin->MakeLinkTo(InCurrentPinNode);
// 				}
// 			}
// 		}
// 		if (OutputLinks[1].Links.Num() > 0&& OutputLinks[1].Links[0].LinkedOp)
// 		{
// 			if(UEdGraphPin*InCurrentPinNode = CastChecked<ULegacySequenceAction>(OutputLinks[1].Links[0].LinkedOp)->GetPinOrCreateNode(InBlueprint,InGraph,OutputLinks[1].Links[0].InputLinkIdx))
// 			{
// 				UEdGraphPin* InputPin = CurrentNode ->FindPin(NAME_CleanedUp);
// 				if (ensure(InputPin))
// 				{
// 					InputPin->MakeLinkTo(InCurrentPinNode);
// 				}
// 			}
// 		}
// 	}
// 	
// 	static FName NAME_Initiate = "Initiate";
// 	static FName NAME_CleanUp = "CleanUp";
// 	
// 	UEdGraphPin* Result =  CurrentNode->FindPin(InputLinkIdx == 0?NAME_Initiate:NAME_CleanUp);
// 	ensure(Result);
// 	return Result;
// }
