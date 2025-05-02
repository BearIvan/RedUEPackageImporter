#include "World/Sequences/Object/LegacySequenceObjects.h"

#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "K2Node_Event.h"
#include "K2Node_Literal.h"
#include "K2Node_MakeStruct.h"
#include "K2Node_VariableGet.h"
#include "Blueprints/RedUEBlueprintFunctionLibrary.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "World/Actors/LegacyActor.h"


UEdGraphPin* ULegacySequenceAction::GetPinOrCreateNode(UBlueprint* InBlueprint, UEdGraph* InGraph, int32 InputLinkIdx)
{
	unimplemented();
	return nullptr;
}

UK2Node* ULegacySequenceEvent::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	unimplemented();
	return nullptr;
}

UK2Node* ULegacySequenceVariable::CreateGetNode(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(FBPVariableDescription* Variable = GetOrCreateVariable(InBlueprint, InGraph))
	{
		FGraphNodeCreator<UK2Node_VariableGet> NodeCreator(*InGraph);
		UK2Node_VariableGet* VariableGet = NodeCreator.CreateNode();
		VariableGet->VariableReference.SetSelfMember(Variable->VarName);
		NodeCreator.Finalize();
		return VariableGet;
	}
	return nullptr;
}

FBPVariableDescription* ULegacySequenceVariable::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	unimplemented();
	return nullptr;
}


UK2Node* ULegacySeqEvent_LevelLoaded::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(CurrentNode)
	{
		return CurrentNode;
	}
	
	FMemberReference EventReference;
	EventReference.SetExternalMember("ReceiveBeginPlay", AActor::StaticClass());
	
	UK2Node_Event* NewEventNode = NewObject<UK2Node_Event>(InGraph);
	NewEventNode->EventReference = EventReference;
	NewEventNode->CreateNewGuid();
	NewEventNode->PostPlacedNewNode();
	NewEventNode->SetFlags(RF_Transactional);
	NewEventNode->AllocateDefaultPins();
	NewEventNode->bCommentBubblePinned = true;
	NewEventNode->bOverrideFunction=true;
	NewEventNode->NodePosY = 0;
	NewEventNode->OnUpdateCommentText(GetLegacyFullName());
	UEdGraphSchema_K2::SetNodeMetaData(NewEventNode, FNodeMetadata::DefaultGraphNode);
	InGraph->AddNode(NewEventNode);
	if(ensure(OutputLinks.Num()==1||OutputLinks.Num()==3))
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

UEdGraphPin* ULegacySeqAct_SetMatInstScalarParam::GetPinOrCreateNode(UBlueprint* InBlueprint, UEdGraph* InGraph, int32 InputLinkIdx)
{
	return Super::GetPinOrCreateNode(InBlueprint, InGraph, InputLinkIdx);
}


// UK2Node* ULegacySeqAct_SetMatInstScalarParam::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
// {
// 	if(CurrentNode)
// 	{
// 		return CurrentNode;
// 	}
// 	if(ensure(VariableLinks.Num() == 2&&InputLinks.Num() == 1&&OutputLinks.Num() == 1)&&ensure(VariableLinks[0].LinkedVariables.Num()==1))
// 	{
// 		check(VariableLinks[0].LinkedVariables[0]);
// 		VariableLinks[0].LinkedVariables[0]->CreateGetNode(InBlueprint,InGraph);
// 		FGraphNodeCreator<UK2Node_CallFunction> NodeCreator(*InGraph);
// 	
// 		UK2Node_CallFunction* TestNode = NodeCreator.CreateNode();
// 		TestNode->SetFromFunction(UKismetSystemLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UKismetSystemLibrary, PrintString)));
// 		TestNode->NodePosX = 200;
// 		NodeCreator.Finalize();
// 		UEdGraphPin* ChooserTablePin = TestNode->FindPin(FName("InString"));
// 		TestNode->GetSchema()->TrySetDefaultValue(*ChooserTablePin, TEXT("Create from C++"));
// 		//FGraphNodeCreator<UK2Node_Literal> NodeCreator(*InGraph);
// 	
// 		return TestNode;
// 	}
// 	return nullptr;
// }

FBPVariableDescription* ULegacySeqVar_Object::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(!VarGuid.IsValid())
	{
		if(VarName == NAME_None)
		{
			VarName = GetLegacyFName();
		}
		if(ObjValue)
		{
			FEdGraphPinType ObjectPinType(UEdGraphSchema_K2::PC_Object, NAME_None, ObjValue->PresentObject->GetClass(), EPinContainerType::None, false, FEdGraphTerminalType());
			if(ensure(FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VarName, ObjectPinType,ObjValue->PresentObject->GetPathName())))
			{
				if(ensure(InBlueprint->NewVariables.Last().VarName == VarName))
				{
					VarGuid = InBlueprint->NewVariables.Last().VarGuid;
				}
			}
		}
		else
		{
			unimplemented();
		}
		if(!VarGuid.IsValid())
		{
			return nullptr;
		}
	}
	return InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });
}

UEdGraphPin* USeqAct_ToggleCinematicMode::GetPinOrCreateNode(UBlueprint* InBlueprint, UEdGraph* InGraph, int32 InputLinkIdx)
{
	if (!ensure(VariableLinks.Num() == 1&&InputLinks.Num() == 3&&OutputLinks.Num() == 3))
	{
		return nullptr;
	}
	if(!ensure(InputLinkIdx<3))
	{
		return nullptr;
	}
	
	if(!CurrentNode)
	{
		FGraphNodeCreator<UK2Node_CallFunction> NodeCreator(*InGraph);
		UK2Node_CallFunction* NewNode = NodeCreator.CreateNode();
		NewNode->SetFromFunction(URedUEBlueprintFunctionLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(URedUEBlueprintFunctionLibrary, ToggleCinematicMode)));
		NodeCreator.Finalize();
		CurrentNode = NewNode;
		{
			FGraphNodeCreator<UK2Node_MakeStruct> NodeStructMakeCreator(*InGraph);
			UK2Node_MakeStruct* NodeStructMake = NodeStructMakeCreator.CreateNode();
			NodeStructMake->StructType = FRedUEToggleCinematicModeParameters::StaticStruct();
			NodeStructMakeCreator.Finalize();
			
			static FName NAME_bDisableMovement("bDisableMovement");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bDisableMovement),  bDisableMovement ? TEXT("true") : TEXT("false"));

			static FName NAME_bDisableTurning("bDisableTurning");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bDisableTurning),  bDisableTurning ? TEXT("true") : TEXT("false"));

			static FName NAME_bHidePlayer("bHidePlayer");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bHidePlayer),  bHidePlayer ? TEXT("true") : TEXT("false"));

			static FName NAME_bDisableInput("bDisableInput");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bDisableInput),  bDisableInput ? TEXT("true") : TEXT("false"));

			static FName NAME_bHideHUD("bHideHUD");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bHideHUD),  bHideHUD ? TEXT("true") : TEXT("false"));
			
			static FName NAME_bDisableAI("bDisableAI");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bDisableAI),  bDisableAI ? TEXT("true") : TEXT("false"));
			
			static FName NAME_bHideFlashlight("bHideFlashlight");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bHideFlashlight),  bHideFlashlight ? TEXT("true") : TEXT("false"));
			
			static FName NAME_bForMovieCapture("bForMovieCapture");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bForMovieCapture),  bForMovieCapture ? TEXT("true") : TEXT("false"));

			static FName NAME_bLowerWeapon("bLowerWeapon");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bLowerWeapon),  bLowerWeapon ? TEXT("true") : TEXT("false"));

			static FName NAME_bRaiseWeapon("bRaiseWeapon");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bRaiseWeapon),  bRaiseWeapon ? TEXT("true") : TEXT("false"));
			
			static FName NAME_bUncrouchPlayer("bUncrouchPlayer");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bUncrouchPlayer),  mbUncrouchPlayer ? TEXT("true") : TEXT("false"));

			if(mCinematicMesh)
			{
				static FName NAME_CinematicMesh("CinematicMesh");
				NodeStructMake->GetSchema()->TrySetDefaultObject(*NodeStructMake->FindPinChecked(NAME_CinematicMesh),  mCinematicMesh->ExportToContent());
			}
			
			static FName NAME_EndWeapon("EndWeapon");
			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_EndWeapon),  mEndWeapon.ToString());

			static FName NAME_Parameters("Parameters");
			NewNode->FindPinChecked(NAME_Parameters)->MakeLinkTo(NodeStructMake->Pins[0]);
		}
	}

	UEdGraphPin* Result =  CurrentNode->FindPin(*InputLinks[InputLinkIdx].LinkDesc);
	ensure(Result);
	return Result;
}

UEdGraphPin* ULegacySeqAct_Interp::GetPinOrCreateNode(UBlueprint* InBlueprint, UEdGraph* InGraph, int32 InputLinkIdx)
{
	return nullptr;
	//return Super::GetPinOrCreateNode(InBlueprint, InGraph, InputLinkIdx);
}
