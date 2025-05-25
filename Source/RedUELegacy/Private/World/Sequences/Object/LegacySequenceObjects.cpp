#include "World/Sequences/Object/LegacySequenceObjects.h"

#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "K2Node_Event.h"
#include "K2Node_ExecutionSequence.h"
#include "K2Node_Literal.h"
#include "K2Node_MakeStruct.h"
#include "K2Node_VariableGet.h"
#include "Blueprint/Kismet/K2Node_SequenceAction.h"
#include "Blueprints/RedUEBlueprintFunctionLibrary.h"
#include "Core/RedUELegacyArchive.h"
#include "Kismet/Base/SequenceAction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "World/Actors/LegacyActor.h"


UK2Node_SequenceAction* ULegacySequenceOp::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return nullptr;
}

UEdGraphPin* ULegacySequenceOp::GetInputPin(int32 Index, UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	UK2Node_SequenceAction* Action = ExportToBlueprint(InBlueprint, InGraph);
	if (Action)
	{
		FName *InputName = Action->LegacyIndexToInputPin.Find(Index);
		if (ensure(InputName))
		{
			return Action->FindPinChecked(*InputName);
		}
	}
	return nullptr;
	
}


UK2Node_SequenceAction* ULegacySequenceImporter::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (CurrentNode)
	{
		return CurrentNode;
	}

	FGraphNodeCreator<UK2Node_SequenceAction> NodeCreator(*InGraph);
	UK2Node_SequenceAction* NewNode = NodeCreator.CreateNode();
	NewNode->Action = NewObject<USequenceAction>(NewNode,ToAction->GetClass(),NAME_None,RF_Transactional,ToAction);
	NodeCreator.Finalize();
	for (int32  i = 0;i<OutputLinks.Num();i++)
	{
		if (FName*OutputName = NewNode->LegacyIndexToOutputPin.Find(i))
		{
			UEdGraphPin*OutputPin = NewNode->FindPinChecked(*OutputName);

			TArray<UEdGraphPin*> Pins;
			for (FLegacySeqOpOutputInputLink& Link : OutputLinks[i].Links)
			{
				if (ULegacySequenceOp* SequenceOp =  CastChecked<ULegacySequenceOp>(OutputLinks[0].Links[0].LinkedOp,ECastCheckedType::NullAllowed))
				{
					if (UEdGraphPin* Pin = SequenceOp->GetInputPin(Link.InputLinkIdx,InBlueprint,InGraph))
					{
						Pins.Add(Pin);
					}
				}
			}
			if (Pins.Num() > 1)
			{
				UK2Node_ExecutionSequence* ExecutionSequence = nullptr;
				int32 ThenPinIndex = 0;
				auto GetNextThenPin = [&ThenPinIndex,&ExecutionSequence]()
				{
					while (! ExecutionSequence->GetThenPinGivenIndex(ThenPinIndex))
					{
						ExecutionSequence->AddInputPin();
					}
					return  ExecutionSequence->GetThenPinGivenIndex(ThenPinIndex++);
				};
				{
					FGraphNodeCreator<UK2Node_ExecutionSequence> ExecutionSequenceCreator(*InGraph);
					ExecutionSequence = ExecutionSequenceCreator.CreateNode();
					NodeCreator.Finalize();
		
					// ExecutionSequence->NodePosX = X;
					// X += 400;
					// ExecutionSequence->NodePosY = Y;
		
					OutputPin->MakeLinkTo(ExecutionSequence->GetExecPin());
				}
				for (UEdGraphPin*Pin : Pins)
				{
					GetNextThenPin()->MakeLinkTo(Pin);
				}
			}
			else if (Pins.Num() == 1)
			{
				OutputPin->MakeLinkTo(Pins[0]);
			}
		}
		
	}
	
	CurrentNode = NewNode;
	return NewNode;
}

void ULegacySequenceImporter::PreLegacySerializeUnrealProps(FRedUELegacyArchive& Ar)
{
	Super::PreLegacySerializeUnrealProps(Ar);
	
	int32 Tell = Ar.Tell();
	LegacySerializeUnrealProps(ToAction->GetClass(),ToAction,Ar);
	Ar.Seek(Tell);
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

UK2Node_SequenceAction* ULegacySeqAct_Interp::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return Super::ExportToBlueprint(InBlueprint, InGraph);
}

UEdGraphPin* ULegacySeqAct_Interp::GetInputPin(int32 Index, UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return Super::GetInputPin(Index, InBlueprint, InGraph);
}

//
// UEdGraphPin* USeqAct_ToggleCinematicMode::GetPinOrCreateNode(UBlueprint* InBlueprint, UEdGraph* InGraph, int32 InputLinkIdx)
// {
// 	if (!ensure(VariableLinks.Num() == 1&&InputLinks.Num() == 3&&OutputLinks.Num() == 3))
// 	{
// 		return nullptr;
// 	}
// 	if(!ensure(InputLinkIdx<3))
// 	{
// 		return nullptr;
// 	}
// 	
// 	if(!CurrentNode)
// 	{
// 		FGraphNodeCreator<UK2Node_CallFunction> NodeCreator(*InGraph);
// 		UK2Node_CallFunction* NewNode = NodeCreator.CreateNode();
// 		NewNode->SetFromFunction(URedUEBlueprintFunctionLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(URedUEBlueprintFunctionLibrary, ToggleCinematicMode)));
// 		NodeCreator.Finalize();
// 		CurrentNode = NewNode;
// 		{
// 			FGraphNodeCreator<UK2Node_MakeStruct> NodeStructMakeCreator(*InGraph);
// 			UK2Node_MakeStruct* NodeStructMake = NodeStructMakeCreator.CreateNode();
// 			NodeStructMake->StructType = FRedUEToggleCinematicModeParameters::StaticStruct();
// 			NodeStructMakeCreator.Finalize();
// 			
// 			static FName NAME_bDisableMovement("bDisableMovement");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bDisableMovement),  bDisableMovement ? TEXT("true") : TEXT("false"));
//
// 			static FName NAME_bDisableTurning("bDisableTurning");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bDisableTurning),  bDisableTurning ? TEXT("true") : TEXT("false"));
//
// 			static FName NAME_bHidePlayer("bHidePlayer");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bHidePlayer),  bHidePlayer ? TEXT("true") : TEXT("false"));
//
// 			static FName NAME_bDisableInput("bDisableInput");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bDisableInput),  bDisableInput ? TEXT("true") : TEXT("false"));
//
// 			static FName NAME_bHideHUD("bHideHUD");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bHideHUD),  bHideHUD ? TEXT("true") : TEXT("false"));
// 			
// 			static FName NAME_bDisableAI("bDisableAI");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bDisableAI),  bDisableAI ? TEXT("true") : TEXT("false"));
// 			
// 			static FName NAME_bHideFlashlight("bHideFlashlight");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bHideFlashlight),  bHideFlashlight ? TEXT("true") : TEXT("false"));
// 			
// 			static FName NAME_bForMovieCapture("bForMovieCapture");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bForMovieCapture),  bForMovieCapture ? TEXT("true") : TEXT("false"));
//
// 			static FName NAME_bLowerWeapon("bLowerWeapon");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bLowerWeapon),  bLowerWeapon ? TEXT("true") : TEXT("false"));
//
// 			static FName NAME_bRaiseWeapon("bRaiseWeapon");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bRaiseWeapon),  bRaiseWeapon ? TEXT("true") : TEXT("false"));
// 			
// 			static FName NAME_bUncrouchPlayer("bUncrouchPlayer");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_bUncrouchPlayer),  mbUncrouchPlayer ? TEXT("true") : TEXT("false"));
//
// 			if(mCinematicMesh)
// 			{
// 				static FName NAME_CinematicMesh("CinematicMesh");
// 				NodeStructMake->GetSchema()->TrySetDefaultObject(*NodeStructMake->FindPinChecked(NAME_CinematicMesh),  mCinematicMesh->ExportToContent());
// 			}
// 			
// 			static FName NAME_EndWeapon("EndWeapon");
// 			NodeStructMake->GetSchema()->TrySetDefaultValue(*NodeStructMake->FindPinChecked(NAME_EndWeapon),  mEndWeapon.ToString());
//
// 			static FName NAME_Parameters("Parameters");
// 			NewNode->FindPinChecked(NAME_Parameters)->MakeLinkTo(NodeStructMake->Pins[0]);
// 		}
// 	}
//
// 	UEdGraphPin* Result =  CurrentNode->FindPin(*InputLinks[InputLinkIdx].LinkDesc);
// 	ensure(Result);
// 	return Result;
// }
