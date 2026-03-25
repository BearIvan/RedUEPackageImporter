#include "World/Sequences/Object/LegacySequenceObjects.h"

#include "CurveEditorTypes.h"
#include "EdGraphSchema_K2.h"
#include "Editor.h"
#include "IDetailTreeNode.h"
#include "ISequencerModule.h"
#include "K2Node_CallFunction.h"
#include "K2Node_CustomEvent.h"
#include "K2Node_Event.h"
#include "K2Node_ExecutionSequence.h"
#include "K2Node_Literal.h"
#include "K2Node_MakeStruct.h"
#include "K2Node_VariableGet.h"
#include "LevelSequence.h"
#include "MovieScene.h"
#include "MovieSceneEventUtils.h"
#include "ObjectTools.h"
#include "PackageTools.h"
#include "SequencerUtilities.h"
#include "Actors/MaterialInstanceActor/MaterialInstanceHybridActor.h"
#include "Animation/AnimSequence.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/Kismet/K2Node_SequenceAction.h"
#include "Blueprints/RedUEBlueprintFunctionLibrary.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacySubsystem.h"
#include "Kismet/EngineSequenceActions.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Kismet/Base/SequenceAction.h"
#include "Kismet/Gameplay/SeqAct_Interp.h"
#include "Kismet/Gameplay/XSeqAct_InstancePattern.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Materials/Hybrid/MaterialInstanceHybrid.h"
#include "Materials/Hybrid/MovieSceneHybridMaterialParameterSection.h"
#include "Materials/Hybrid/MovieSceneHybridMaterialTrack.h"
#include "Mesh/LegacyAnimSequence.h"
#include "Mesh/LegacyAnimSet.h"
#include "Sections/MovieSceneAudioSection.h"
#include "Sections/MovieSceneEventTriggerSection.h"
#include "Sections/MovieSceneParticleSection.h"
#include "Sections/MovieSceneVisibilitySection.h"
#include "Sounds/LegacyXSound.h"
#include "Tracks/MovieScene3DTransformTrack.h"
#include "Tracks/MovieSceneAudioTrack.h"
#include "Tracks/MovieSceneEventTrack.h"
#include "Tracks/MovieSceneParticleParameterTrack.h"
#include "Tracks/MovieSceneParticleTrack.h"
#include "Tracks/MovieSceneSkeletalAnimationTrack.h"
#include "Tracks/MovieSceneVisibilityTrack.h"
#include "World/LegacyWorld.h"
#include "World/Actors/LegacyActor.h"
#include "World/Sequences/LegacySequence.h"


UK2Node* ULegacySequenceObject::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return nullptr;
}

UEdGraphPin* ULegacySequenceOp::GetInputPin(int32 Index, UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (UK2Node_SequenceAction* Action = Cast<UK2Node_SequenceAction>( ExportToBlueprint(InBlueprint, InGraph)))
	{
		FName *InputName = Action->LegacyIndexToInputPin.Find(Index);
		if (ensure(InputName))
		{
			return Action->FindPinChecked(*InputName);
		}
	}
	return nullptr;
	
}

UEdGraphPin* ULegacySequenceOp::GetEventPin(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (UK2Node_SequenceAction* Action = Cast<UK2Node_SequenceAction>( ExportToBlueprint(InBlueprint, InGraph)))
	{
		return Action->GetEventOutput();
	}
	return nullptr;
}

void ULegacySequenceOp::SimulatedImport()
{
	
}


void ULegacySequenceOp::FillPin(UBlueprint* InBlueprint, UEdGraph* InGraph, const FLegacySeqOpOutputLink& LegacySeqOpOutputLink, UEdGraphPin* OutputPin)
{
	TArray<UEdGraphPin*> Pins;
	for (const FLegacySeqOpOutputInputLink& Link : LegacySeqOpOutputLink.Links)
	{
		if (ULegacySequenceOp* SequenceOp =  CastChecked<ULegacySequenceOp>(Link.LinkedOp,ECastCheckedType::NullAllowed))
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
			ExecutionSequenceCreator.Finalize();
		
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

void ULegacySequenceImporter::FillAction(USequenceAction* InSequenceAction)
{
	
}

UK2Node* ULegacySequenceImporter::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (CurrentNode)
	{
		return CurrentNode;
	}

	FGraphNodeCreator<UK2Node_SequenceAction> NodeCreator(*InGraph);
	UK2Node_SequenceAction* NewNode = NodeCreator.CreateNode();

	NewNode->Action = NewObject<USequenceAction>(NewNode,ToAction->GetClass(),NAME_None,RF_Transactional,ToAction);
	FillAction(NewNode->Action);
	NewNode->bCommentBubblePinned = true;
	NewNode->NodeComment = GetLegacyFullName();
	
	NodeCreator.Finalize();
	CurrentNode = NewNode;

	if (EventLinks.Num() > 0)
	{
		if (ensure(NewNode->InputEventsName != NAME_None))
		{
			ensure(EventLinks.Num() == 1);
			for (ULegacySequenceOp* LinkEvent : EventLinks[0].LinkedEvents)
			{
				NewNode->AddEventPin();
			}
			TArray<UEdGraphPin*> Pins;
			NewNode->GetEventPins(Pins);
			for (ULegacySequenceOp* LinkEvent : EventLinks[0].LinkedEvents)
			{
				if (LinkEvent)
				{
					if (UEdGraphPin* Pin = LinkEvent->GetEventPin(InBlueprint,InGraph);ensure(Pin))
					{
						Pins[0]->MakeLinkTo(Pin);
						Pins.RemoveAtSwap(0);
					}
				}
			}
		}
	}
	
	for (int32  i = 0;i<OutputLinks.Num();i++)
	{
		
		const FLegacySeqOpOutputLink& LegacySeqOpOutputLink = OutputLinks[i];
		if (FName*OutputName = NewNode->LegacyIndexToOutputPin.Find(i))
		{
			UEdGraphPin*OutputPin = NewNode->FindPinChecked(*OutputName);

			FillPin(InBlueprint, InGraph, LegacySeqOpOutputLink, OutputPin);
		}
	}
	

	for (int32  i = 0;i<VariableLinks.Num();i++)
	{
		if (FName*VariableName = NewNode->LegacyIndexToVariableName.Find(i))
		{
			TArray< ULegacySequenceVariable*> LegacySequenceVariables;
			
			TFunction<void(ULegacySequenceVariable*)> LambdaBuildVariableLinks = [&LegacySequenceVariables,&LambdaBuildVariableLinks](ULegacySequenceVariable* InLinkVariable)
			{
				if (ULegacySeqVar_External* SeqVar_External = Cast<ULegacySeqVar_External>(InLinkVariable))
				{
					for (ULegacySequenceVariable* InLinkVariableFromExternal :SeqVar_External->Variables)
					{
						LambdaBuildVariableLinks(InLinkVariableFromExternal);
					}
				}
				else if (ULegacySeqVar_Named* SeqVar_Named = Cast<ULegacySeqVar_Named>(InLinkVariable))
				{
					LambdaBuildVariableLinks(SeqVar_Named->FindVariable());
				}
				else
				{
					LegacySequenceVariables.Add(InLinkVariable);
				}
			};
			for (ULegacySequenceVariable* InLinkVariable: VariableLinks[i].LinkedVariables)
			{
				LambdaBuildVariableLinks(InLinkVariable);
			}
			
			if (LegacySequenceVariables.Num() > 0)
			{
				FProperty* DstProperty = NewNode->Action->GetClass()->FindPropertyByName(*VariableName);
				if (!ensure(DstProperty))
				{
					continue;
				}
				
				for (int32  LinkID = 0;LinkID<LegacySequenceVariables.Num();LinkID++)
				{
					if (FArrayProperty* DstArrayProperty = CastField<FArrayProperty>(DstProperty))
					{
						if (ULegacySequenceVariable* Variable = CastChecked<ULegacySequenceVariable>(LegacySequenceVariables[LinkID],ECastCheckedType::NullAllowed))
						{
							if (FName VariableDescription = Variable->GetOrCreateVariable(InBlueprint,InGraph);VariableDescription != NAME_None)
							{
								FProperty* BlueprintProperty = InBlueprint->SkeletonGeneratedClass->FindPropertyByName(VariableDescription);
								if (ensure(BlueprintProperty))
								{
									if (LegacySequenceVariables.Num() == 0 && PropertyAccessUtil::ArePropertiesCompatible(BlueprintProperty,DstProperty) )
									{
										FMemberReference&Reference = NewNode->Action->PropertiesReference.Add(*VariableName);
										Reference.SetExternalMember(VariableDescription,InBlueprint->SkeletonGeneratedClass);
									}
									else
									{
										if (ensure(PropertyAccessUtil::ArePropertiesCompatible(BlueprintProperty,DstProperty) || PropertyAccessUtil::ArePropertiesCompatible(BlueprintProperty,DstArrayProperty->Inner)))
										{
											FScriptArrayHelper ArrayHelper(DstArrayProperty,DstArrayProperty->ContainerPtrToValuePtr<void>(NewNode->Action));
											FSequenceActionPropertyArrayReference& PropertyArrayReference = NewNode->Action->PropertiesArrayReference.FindOrAdd(*VariableName);
											PropertyArrayReference.ArrayElementReference.Add(ArrayHelper.Num()).SetExternalMember(VariableDescription,InBlueprint->SkeletonGeneratedClass);
											ArrayHelper.AddValue();
										}
									}
								}
							}
						}
					}
					else
					{
						ensure(VariableLinks[i].LinkedVariables.Num() == 1);
						if (ULegacySequenceVariable* Variable = CastChecked<ULegacySequenceVariable>(LegacySequenceVariables[0],ECastCheckedType::NullAllowed))
						{
							if (FName VariableDescription = Variable->GetOrCreateVariable(InBlueprint,InGraph);VariableDescription != NAME_None)
							{
								FProperty* BlueprintProperty = InBlueprint->SkeletonGeneratedClass->FindPropertyByName(VariableDescription);
								if (ensure(BlueprintProperty))
								{
									if (ensure(PropertyAccessUtil::ArePropertiesCompatible(BlueprintProperty,DstProperty)))
									{
										FMemberReference&Reference = NewNode->Action->PropertiesReference.Add(*VariableName);
										Reference.SetExternalMember(VariableDescription,InBlueprint->SkeletonGeneratedClass);
									}
								}
							}
						
						}
					}
				}
			}
			
		}
	}
	
	return NewNode;
}

void ULegacySequenceImporter::PreLegacySerializeUnrealProps(FRedUELegacyArchive& Ar)
{
	Super::PreLegacySerializeUnrealProps(Ar);
	if (bNeedSerializeToActon)
	{
		int32 Tell = Ar.Tell();
		LegacySerializeUnrealProps(ToAction->GetClass(),ToAction,Ar);
		Ar.Seek(Tell);
	}

}


UEdGraphPin* ULegacySeqAct_FinishSequence::GetInputPin(int32 Index, UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (Index == 0 && CurrentTunnelNode)
	{
		if (UEdGraphPin* OutputPin = CurrentTunnelNode->FindPinChecked(InputPinName))
		{
			return OutputPin;
		}
	}
	return nullptr;
}

UEdGraphPin* ULegacySeqAct_FinishSequence::GetEventPin(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	ensure(!"not_implemented");
	return nullptr;
}

UK2Node* ULegacySeqAct_FinishSequence::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return nullptr;
}

UEdGraphPin* ULegacySeqEvent_SequenceActivated::GetInputPin(int32 Index, UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	ensure(!"not_implemented");
	return nullptr;
}

UEdGraphPin* ULegacySeqEvent_SequenceActivated::GetEventPin(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	ensure(!"not_implemented");
	return nullptr;
}

UK2Node* ULegacySeqEvent_SequenceActivated::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (OutputPinName != NAME_None && CurrentTunnelNode && OutputLinks.Num() == 1)
	{
		if (UEdGraphPin* OutputPin = CurrentTunnelNode->FindPinChecked(OutputPinName))
		{
			const FLegacySeqOpOutputLink& LegacySeqOpOutputLink = OutputLinks[0];
			FillPin(InBlueprint, InGraph, LegacySeqOpOutputLink, OutputPin);
		}
	}
	return nullptr;
}

FName ULegacySequenceVariable::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return NAME_None;
}

void ULegacySequenceVariable::Fill(ALegacyKismet* Kismet)
{
}


FName ULegacySeqVar_Object::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(!VarGuid.IsValid())
	{
		if(VarName == NAME_None)
		{
			VarName = GetLegacyFName();
		}
		{
			ensure(!ObjValue||ObjValue->IsA<ULegacyActor>());
			FEdGraphPinType ObjectPinType(UEdGraphSchema_K2::PC_Object, NAME_None, AActor::StaticClass(), EPinContainerType::None, false, FEdGraphTerminalType());
			if(ensure(FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VarName, ObjectPinType)))
			{
				if(ensure(InBlueprint->NewVariables.Last().VarName == VarName))
				{
					InBlueprint->NewVariables.Last().PropertyFlags &= ~CPF_DisableEditOnInstance;
					VarGuid = InBlueprint->NewVariables.Last().VarGuid;
				}
			}
		}
		if(!VarGuid.IsValid())
		{
			return NAME_None;
		}
	}
	FBPVariableDescription* PointerPtr = InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });
	if(!PointerPtr)
	{
		return NAME_None;
	}
	return PointerPtr->VarName;
}

void ULegacySeqVar_Object::Fill(ALegacyKismet* Kismet)
{
	if (VarName == NAME_None || !VarGuid.IsValid())
	{
		return;
	}
	FProperty* InProperty = Kismet->GetClass()->FindPropertyByName(VarName);
	if (ensure(InProperty))
	{
		FObjectProperty* InPropertyProperty = CastField<FObjectProperty>(InProperty);
		if (ensure(InPropertyProperty))
		{
			if (ObjValue&&ObjValue->PresentObject)
			{
				InPropertyProperty->SetPropertyValue_InContainer(Kismet, ObjValue->PresentObject);
			}
		}
	}
}

FName ULegacySeqVar_Bool::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(!VarGuid.IsValid())
	{
		if(VarName == NAME_None)
		{
			VarName = GetLegacyFName();
		}
		{
			FEdGraphPinType ObjectPinType(UEdGraphSchema_K2::PC_Boolean, NAME_None,nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
			if(ensure(FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VarName, ObjectPinType,bValue?TEXT("true"):TEXT("false"))))
			{
				if(ensure(InBlueprint->NewVariables.Last().VarName == VarName))
				{
					InBlueprint->NewVariables.Last().PropertyFlags &= ~CPF_DisableEditOnInstance;
					VarGuid = InBlueprint->NewVariables.Last().VarGuid;
				}
			}
		}
		if(!VarGuid.IsValid())
		{
			return NAME_None;
		}
	}
	FBPVariableDescription* PointerPtr = InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });
	if(!PointerPtr)
	{
		return NAME_None;
	}
	return PointerPtr->VarName;
}

void ULegacySeqVar_Bool::Fill(ALegacyKismet* Kismet)
{
	Super::Fill(Kismet);
}

FName ULegacySeqVar_Int::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(!VarGuid.IsValid())
	{
		if(VarName == NAME_None)
		{
			VarName = GetLegacyFName();
		}
		{
			FEdGraphPinType ObjectPinType(UEdGraphSchema_K2::PC_Int, NAME_None,nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
			if(ensure(FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VarName, ObjectPinType,FString::FromInt(IntValue))))
			{
				if(ensure(InBlueprint->NewVariables.Last().VarName == VarName))
				{
					InBlueprint->NewVariables.Last().PropertyFlags &= ~CPF_DisableEditOnInstance;
					VarGuid = InBlueprint->NewVariables.Last().VarGuid;
				}
			}
		}
		if(!VarGuid.IsValid())
		{
			return NAME_None;
		}
	}
	FBPVariableDescription* PointerPtr = InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });
	if(!PointerPtr)
	{
		return NAME_None;
	}
	return PointerPtr->VarName;
}

void ULegacySeqVar_Int::Fill(ALegacyKismet* Kismet)
{
	Super::Fill(Kismet);
}



FName ULegacySeqVar_Float::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(!VarGuid.IsValid())
	{
		if(VarName == NAME_None)
		{
			VarName = GetLegacyFName();
		}
		{
			FEdGraphPinType ObjectPinType(UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double,nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
			if(ensure(FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VarName, ObjectPinType,FString::Printf(TEXT("%f"),FloatValue))))
			{
				if(ensure(InBlueprint->NewVariables.Last().VarName == VarName))
				{
					InBlueprint->NewVariables.Last().PropertyFlags &= ~CPF_DisableEditOnInstance;
					VarGuid = InBlueprint->NewVariables.Last().VarGuid;
				}
			}
		}
		if(!VarGuid.IsValid())
		{
			return NAME_None;
		}
	}
	FBPVariableDescription* PointerPtr = InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });
	if(!PointerPtr)
	{
		return NAME_None;
	}
	return PointerPtr->VarName;
}


FName ULegacySeqVar_Named::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	checkNoEntry();
	return NAME_None;
}

void ULegacySeqVar_Named::Fill(ALegacyKismet* Kismet)
{
}

ULegacySequenceVariable* ULegacySeqVar_Named::FindVariable()
{
	if (RefVariable)
	{
		return RefVariable;
	}
	URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
	if (RedUELegacySubsystem->GLegacyWorld && RedUELegacySubsystem->GLegacyWorld->PersistentLevel)
	{
		for (ULegacySequence* Sequence: RedUELegacySubsystem->GLegacyWorld->PersistentLevel->GameSequences)
		{
			if (Sequence)
			{
				RefVariable = Sequence->FindSequenceVariable(FindVarName);
				if (RefVariable)
				{
					return RefVariable;
				}
			}
		}
	}
	return nullptr;
}

FName ULegacySeqVar_ObjectList::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(!VarGuid.IsValid())
	{
		if(VarName == NAME_None)
		{
			VarName = GetLegacyFName();
		}
		{
			FEdGraphPinType ObjectPinType(UEdGraphSchema_K2::PC_Object, NAME_None, AActor::StaticClass(), EPinContainerType::Array, false, FEdGraphTerminalType());
			if(ensure(FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VarName, ObjectPinType)))
			{
				if(ensure(InBlueprint->NewVariables.Last().VarName == VarName))
				{
					InBlueprint->NewVariables.Last().PropertyFlags &= ~CPF_DisableEditOnInstance;
					VarGuid = InBlueprint->NewVariables.Last().VarGuid;
				}
			}
		}
		if(!VarGuid.IsValid())
		{
			return NAME_None;
		}
	}
	FBPVariableDescription* VariablePtr = InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });
	if (!VariablePtr)
	{
		return NAME_None;
	}
	return VariablePtr->VarName;
}

void ULegacySeqVar_ObjectList::Fill(ALegacyKismet* Kismet)
{
	if (VarName == NAME_None || !VarGuid.IsValid())
	{
		return;
	}
	FProperty* Property = Kismet->GetClass()->FindPropertyByName(VarName);
	if (ensure(Property))
	{
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property);
		if (ensure(ArrayProperty))
		{
			if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(ArrayProperty->Inner))
			{
				FScriptArrayHelper_InContainer ArrayHelper(ArrayProperty,Kismet);
				ArrayHelper.EmptyAndAddValues(ObjList.Num());
				for (int32 i =0; i < ObjList.Num(); i++)
				{
					if (ObjList[i])
					{
						ObjectProperty->SetObjectPropertyValue(ArrayHelper.GetRawPtr(i),ObjList[i]->PresentObject);
					}
				}
			}
		}
	}
}

FName ULegacySeqVar_External::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	checkNoEntry();
	return NAME_None;
}

void ULegacySeqVar_External::Fill(ALegacyKismet* Kismet)
{
}

FName ULegacyXSeqVar_PlayerController::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return GET_MEMBER_NAME_CHECKED(ALegacyKismet,PlayerController);
}

FName ULegacyXSeqVar_Elizabeth::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return GET_MEMBER_NAME_CHECKED(ABioshockKismet,ElizabethPawn);
}

ULegacySeqAct_Interp::ULegacySeqAct_Interp()
{
	ToAction = CreateDefaultSubobject<USeqAct_Interp>("USeqAct_Interp");
}

UK2Node* ULegacySeqAct_Interp::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	bool NeedCreateEvent = false;
	if (!CurrentNode)
	{
		if (!ensure(VariableLinks.Num() != 0))
		{
			return nullptr;
		}
		if (!ensure(VariableLinks[0].LinkedVariables.Num() == 1))
		{
			return nullptr;
		}

		ULegacyInterpData* InterpData = Cast<ULegacyInterpData>(VariableLinks[0].LinkedVariables[0]);

		if (!ensure(InterpData))
		{
			return nullptr;
		}
		
		if (!ensure(InterpData->OwnerSeqAct_Interp == this||InterpData->OwnerSeqAct_Interp == nullptr))
		{
			return nullptr;
		}
		
		InterpData->OwnerSeqAct_Interp = this;
		
		NeedCreateEvent = true;
	}
	UK2Node_SequenceAction*  Result =  CastChecked<UK2Node_SequenceAction>(Super::ExportToBlueprint(InBlueprint, InGraph),ECastCheckedType::NullAllowed);
	if (NeedCreateEvent)
	{
		
		for (int32  i = 5;i<OutputLinks.Num();i++)
		{
			UK2Node_CustomEvent* NewEventNode = NewObject<UK2Node_CustomEvent>(InGraph);

			FString EventName = OutputLinks[i].XLinkName.ToString();
			if (EventName.IsEmpty())
			{
				EventName = *OutputLinks[i].LinkDesc;
			}
			EventName = GetLegacyName() + TEXT("_") + EventName;
			NewEventNode->CustomFunctionName = *EventName;
			EventName2FunctionName.Add(OutputLinks[i].XLinkName,NewEventNode->CustomFunctionName);
			NewEventNode->CreateNewGuid();
			NewEventNode->PostPlacedNewNode();
			NewEventNode->SetFlags(RF_Transactional);
			NewEventNode->AllocateDefaultPins();
			NewEventNode->bCommentBubblePinned = true;
			NewEventNode->NodePosY = 0;
			NewEventNode->NodeComment = GetLegacyFullName();
			//NewEventNode->OnUpdateCommentText(GetLegacyFullName());
			UEdGraphSchema_K2::SetNodeMetaData(NewEventNode, FNodeMetadata::DefaultGraphNode);
			InGraph->AddNode(NewEventNode);
			FillPin(InBlueprint,InGraph,OutputLinks[i],NewEventNode ->GetThenPin());			
		}
		
	}
	return Result;
}

UEdGraphPin* ULegacySeqAct_Interp::GetInputPin(int32 Index, UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return Super::GetInputPin(Index, InBlueprint, InGraph);
}

void ULegacySeqAct_Interp::SimulatedImport()
{
	Super::SimulatedImport();

	if (!ensure(VariableLinks.Num() != 0))
	{
		return;
	}
	if (!ensure(VariableLinks[0].LinkedVariables.Num() == 1))
	{
		return;
	}

	ULegacyInterpData* InterpData = Cast<ULegacyInterpData>(VariableLinks[0].LinkedVariables[0]);

	if (!ensure(InterpData))
	{
		return;
	}
		
	if (!ensure(InterpData->OwnerSeqAct_Interp == this||InterpData->OwnerSeqAct_Interp == nullptr))
	{
		return;
	}
		
	InterpData->OwnerSeqAct_Interp = this;
}

class UXPatternCommandBase* ULegacyXPatternCommand_ActivateNamedPatternKismetEvent::MakeCommand(class UXSeqAct_InstancePattern* InstancePattern)
{
	if (ULegacyXSeqAct_InstancePattern* Owner = GetTypedOuter<ULegacyXSeqAct_InstancePattern>())
	{
		if (FName* FullName = Owner->EventNameToFullName.Find(KismetPatternEventName))
		{
			UXPatternCommand_KismetEvent* Result = NewObject<UXPatternCommand_KismetEvent>(InstancePattern,NAME_None,RF_Transactional);
			Result->EventName = *FullName;
			return Result;
		}
	}
	return Super::MakeCommand(InstancePattern);
}

class UXPatternCommandBase* ULegacyXPatternCommand_Base::MakeCommand(class UXSeqAct_InstancePattern* InstancePattern)
{
	return nullptr;
}

class UXPatternObjectSetBase* ULegacyXPatternUObjectSet_ElizabethPawns::MakeObjectSet(class UXSeqAct_InstancePattern* InstancePattern)
{
	return NewObject<UXPatternObjectSet_ElizabethPawn>(InstancePattern,NAME_None,RF_Transactional);
}

class UXPatternObjectSetBase* ULegacyXPatternUObjectSet_PlayerPawns::MakeObjectSet(class UXSeqAct_InstancePattern* InstancePattern)
{
	return NewObject<UXPatternObjectSet_Player>(InstancePattern,NAME_None,RF_Transactional);
}

class UXPatternObjectSetBase* ULegacyXPatternUObjectSetBase::MakeObjectSet(class UXSeqAct_InstancePattern* InstancePattern)
{
	return nullptr;
}

class UXPatternCommandBase* ULegacyXPatternCommand_PlaySound::MakeCommand(class UXSeqAct_InstancePattern* InstancePattern)
{
	UXPatternCommand_PlayAudio* Result = NewObject<UXPatternCommand_PlayAudio>(InstancePattern,NAME_None,RF_Transactional);
	if (PlaySound->PlayEvent)
	{
		Result->Audio =  Cast<USoundBase>(PlaySound->PlayEvent->ExportToContent());
	}
	if (Targets)
	{
		Result->Target = Targets->MakeObjectSet(InstancePattern);
	}
	return Result;
}

class UXPatternCommandBase* ULegacyXPatternEvent_AudioTimeElapsed::MakeCommand(class UXSeqAct_InstancePattern* InstancePattern)
{
	UXPatternCommand_AudioTimeElapsed* Result = NewObject<UXPatternCommand_AudioTimeElapsed>(InstancePattern,NAME_None,RF_Transactional);
	Result->bUseAudioTime = bUseAudioTime;
	Result->SleepDurationSeconds = SleepDurationSeconds;
	return Result;
}

class UXPatternCommandBase* ULegacyXPatternEvent_TimeElapsed::MakeCommand(class UXSeqAct_InstancePattern* InstancePattern)
{
	UXPatternCommand_TimeElapsed* Result = NewObject<UXPatternCommand_TimeElapsed>(InstancePattern,NAME_None,RF_Transactional);
	Result->bUseAudioTime = bUseAudioTime;
	Result->SleepDurationSeconds = SleepDurationSeconds;
	return Result;
}

class UXPatternCommandBase* ULegacyXPatternCommand_PlaySpeech::MakeCommand(class UXSeqAct_InstancePattern* InstancePattern)
{
	UXPatternCommand_Speak* Result = NewObject<UXPatternCommand_Speak>(InstancePattern,NAME_None,RF_Transactional);
	
	if (PossibleSpeakers)
	{
		Result->Speaker = PossibleSpeakers->MakeObjectSet(InstancePattern);
		if ( SpeechRequest->PlayEvent)
		{
			Result->Audio = Cast<USoundBase>(SpeechRequest->PlayEvent->ExportToContent());
		}
	}
	return Result;
}

void ULegacyXPattern_AutomaticallyRun::ExportToInstancePattern(class UXSeqAct_InstancePattern* InstancePattern)
{
	for (ULegacyXPatternCommand_Base* Command : Sequence)
	{
		if (Command)
		{
			if (UXPatternCommandBase* NewCommand =  Command->MakeCommand(InstancePattern))
			{
				InstancePattern->Commands.Add(NewCommand);
			}
		}
	}
}

ULegacyXSeqAct_InstancePattern::ULegacyXSeqAct_InstancePattern()
{
	ToAction = CreateDefaultSubobject<UXSeqAct_InstancePattern>("XSeqAct_InstancePattern");
}

UK2Node* ULegacyXSeqAct_InstancePattern::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	bool NeedCreateEvent = false;
	if (!CurrentNode)
	{
		for (int32  i = 4;i<OutputLinks.Num();i++)
		{
			FString EventName = OutputLinks[i].XLinkName.ToString();
			if (EventName.IsEmpty())
			{
				EventName = *OutputLinks[i].LinkDesc;
			}
			FName FirstName = *EventName;
			EventName = GetLegacyName() + TEXT("_") + EventName;
			EventNameToFullName.Add(FirstName, *EventName);
		}
		NeedCreateEvent = true;
	}	
	UK2Node_SequenceAction*  Result = CastChecked<UK2Node_SequenceAction>(Super::ExportToBlueprint(InBlueprint, InGraph),ECastCheckedType::NullAllowed);
	if (NeedCreateEvent)
	{
		for (int32  i = 4;i<OutputLinks.Num();i++)
		{
			UK2Node_CustomEvent* NewEventNode = NewObject<UK2Node_CustomEvent>(InGraph);

			FString EventName = OutputLinks[i].XLinkName.ToString();
			if (EventName.IsEmpty())
			{
				EventName = *OutputLinks[i].LinkDesc;
			}
			EventName = GetLegacyName() + TEXT("_") + EventName;
			NewEventNode->CustomFunctionName = *EventName;
			NewEventNode->CreateNewGuid();
			NewEventNode->PostPlacedNewNode();
			NewEventNode->SetFlags(RF_Transactional);
			NewEventNode->AllocateDefaultPins();
			NewEventNode->bCommentBubblePinned = true;
			NewEventNode->NodePosY = 0;
			NewEventNode->NodeComment = GetLegacyFullName();
			//NewEventNode->OnUpdateCommentText(GetLegacyFullName());
			UEdGraphSchema_K2::SetNodeMetaData(NewEventNode, FNodeMetadata::DefaultGraphNode);
			InGraph->AddNode(NewEventNode);
			FillPin(InBlueprint,InGraph,OutputLinks[i],NewEventNode ->GetThenPin());
		}
	}
	return Result;
}

void ULegacyXSeqAct_InstancePattern::FillAction(USequenceAction* InSequenceAction)
{
	Super::FillAction(InSequenceAction);
	UXSeqAct_InstancePattern *InstancePattern = CastChecked<UXSeqAct_InstancePattern>(InSequenceAction);
	if (BasePatternArchetype)
	{
		BasePatternArchetype->ExportToInstancePattern(InstancePattern);
	}
}

ULegacySeqAct_SetMaterial::ULegacySeqAct_SetMaterial()
{
	ToAction = CreateDefaultSubobject<USeqAct_SetMaterial>("SeqAct_SetMaterial");
	bNeedSerializeToActon = false;
}

void ULegacySeqAct_SetMaterial::FillAction(USequenceAction* InSequenceAction)
{
	Super::FillAction(InSequenceAction);
	USeqAct_SetMaterial* InSeqAct_SetMaterial = CastChecked<USeqAct_SetMaterial>(InSequenceAction);
	if (NewMaterial)
	{
		InSeqAct_SetMaterial->NewMaterial = Cast<UMaterialInterface>(NewMaterial->ExportToContent());
	}
	InSeqAct_SetMaterial->MaterialIndex = MaterialIndex;
}

ULegacyXSeqAct_PlaySound::ULegacyXSeqAct_PlaySound()
{
	ToAction = CreateDefaultSubobject<UXSeqAct_PlaySound>("XSeqAct_PlaySound");
	bNeedSerializeToActon = false;
}

void ULegacyXSeqAct_PlaySound::FillAction(USequenceAction* InSequenceAction)
{
	Super::FillAction(InSequenceAction);
	UXSeqAct_PlaySound* InSeqAct_PlaySound = CastChecked<UXSeqAct_PlaySound>(InSequenceAction);
	if (PlaySound && PlaySound->PlayEvent)
	{
		InSeqAct_PlaySound->PlaySound = Cast<USoundBase>(PlaySound->PlayEvent->ExportToContent());
	}
	if (PlaySound && PlaySound->StopEvent)
	{
		InSeqAct_PlaySound->StopSound = Cast<USoundBase>(PlaySound->StopEvent->ExportToContent());
	}
}


void ULegacyInterpTrack::ExportToLevelSequence( const TSharedRef<ISequencer>&Sequencer, ULegacyActor* LegacyAction)
{
}

void ULegacyInterpTrackEvent::ExportToLevelSequence(const TSharedRef<ISequencer>& Sequencer, ULegacyActor* LegacyAction)
{	
	ULegacyInterpGroup* InterpGroup = GetTypedOuter<ULegacyInterpGroup>();
	if (!ensure(InterpGroup))
	{
		return;
	}
	
	ULegacyInterpData* InterpData = GetTypedOuter<ULegacyInterpData>();
	if (!ensure(InterpData))
	{
		return;
	}
	FGuid ObjectGuid = InterpData->FindOrCreateBinding(*InterpData->CurrentKismet,InterpGroup->GroupName.ToString());
	UMovieSceneEventTrack* Track = InterpData->FindOrCreateTrack<UMovieSceneEventTrack>(ObjectGuid, *InterpGroup->GroupName.ToString());
	Track->SetDisplayName(FText::FromString(InterpGroup->GroupName.ToString()));
	UMovieSceneEventTriggerSection* Section = CastChecked<UMovieSceneEventTriggerSection>(Track->CreateNewSection());
	Track->AddSection(*Section);
	Section->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;
	
	FMovieSceneSequenceEditor* SequenceEditor = FMovieSceneSequenceEditor::Find(Sequencer->GetRootMovieSceneSequence());
	if (!ensure(SequenceEditor))
	{
		return;
	}
	
	UBlueprint* SequenceDirectorBP = SequenceEditor->GetOrCreateDirectorBlueprint(Sequencer->GetRootMovieSceneSequence());
	if (!ensure(SequenceDirectorBP))
	{
		
		return;
	}

	TMovieSceneChannelData<FMovieSceneEvent> ChannelData = Section->EventChannel.GetData();
	ChannelData.Reset();
	for (const FLegacyEventTrackKey& Key :EventTrack)
	{
		FMovieSceneEvent NewKey;

		{
			Section->Modify();
			SequenceDirectorBP->Modify();

			// Ensure the section is bound to the blueprint function generation event
			FMovieSceneEventUtils::BindEventSectionToBlueprint(Section, SequenceDirectorBP);

			// Create the new user-facing event node
			FMovieSceneDirectorBlueprintEndpointDefinition EndpointDefinition = FMovieSceneEventUtils::GenerateEventDefinition(Track);
			EndpointDefinition.EndpointName = Key.EventName.ToString();

			if (UK2Node_CustomEvent* NewEventNode = FMovieSceneDirectorBlueprintUtils::CreateEventEndpoint(SequenceDirectorBP, EndpointDefinition))
			{
				// Bind the node to the event entry point
				UEdGraphPin* BoundObjectPin = FMovieSceneDirectorBlueprintUtils::FindCallTargetPin(NewEventNode, EndpointDefinition.PossibleCallTargetClass);
				FMovieSceneEventUtils::SetEndpoint(&NewKey, Section, NewEventNode, BoundObjectPin);
				if (FName* FunctionName = InterpData->OwnerSeqAct_Interp->EventName2FunctionName.Find(Key.EventName))
				{
					FGraphNodeCreator<UK2Node_CallFunction> NodeCreator(*NewEventNode->GetGraph());
					UK2Node_CallFunction* NextLogicStateNode = NodeCreator.CreateNode();
					NextLogicStateNode->SetFromFunction(InterpData->CurrentKismet->GetClass()->FindFunctionByName(*FunctionName));
					NodeCreator.Finalize();
					NextLogicStateNode->NodePosX = NewEventNode->NodePosX + 450;
					NextLogicStateNode->NodePosY = NewEventNode->NodePosY;
					NewEventNode->GetThenPin()->MakeLinkTo(NextLogicStateNode->GetExecPin());
					UEdGraphPin* NewNodeReturnValuePin =  nullptr;
					for (UEdGraphPin* Pin : NewEventNode->Pins)
					{
						if ((EGPD_Output == Pin->Direction) && Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Object)
						{
							NewNodeReturnValuePin = Pin;
							break;
						}
					}
					UEdGraphPin* OutputPin = NextLogicStateNode->FindPin(UEdGraphSchema_K2::PN_Self, EGPD_Input);
					if (OutputPin && NewNodeReturnValuePin)
					{
						NewNodeReturnValuePin->MakeLinkTo(OutputPin);
					}

					
				}
				//NewEventNode->GetGraph();
				
				
			}
		}
		ChannelData.AddKey(Sequencer->GetRootTickResolution().AsFrameNumber(Key.Time),NewKey);
	}
	
}

void ULegacyInterpTrackToggle::ExportToLevelSequence(const TSharedRef<ISequencer>& Sequencer, ULegacyActor* LegacyAction)
{
	if (!LegacyAction)
	{
		return;
	}
	
	AActor * Actor = Cast<AActor>(LegacyAction->PresentObject);
	if (!Actor)
	{
		return;
	}
	
	if (!Actor->GetRootComponent())
	{
		return;
	}
	
	ULegacyInterpGroup* InterpGroup = GetTypedOuter<ULegacyInterpGroup>();
	if (!ensure(InterpGroup))
	{
		return;
	}
	
	ULegacyInterpData* InterpData = GetTypedOuter<ULegacyInterpData>();
	if (!ensure(InterpData))
	{
		return;
	}
	
	
	FGuid ObjectGuid = InterpData->FindOrCreateBinding(*Actor,InterpGroup->GroupName.ToString());
	UMovieSceneParticleTrack* Track = InterpData->FindOrCreateTrack<UMovieSceneParticleTrack>(ObjectGuid);
	FName PropertyName =  AActor::GetHiddenPropertyName();
//	Track->SetPropertyNameAndPath( PropertyName, PropertyName.ToString());
	UMovieSceneParticleSection* Section =nullptr;
	{
		UMovieSceneSection* NewSection = Track->CreateNewSection();
		ensureAlwaysMsgf(NewSection->HasAnyFlags(RF_Transactional), TEXT("CreateNewSection must return an instance with RF_Transactional set! (pass RF_Transactional to NewObject)"));
		NewSection->SetFlags(RF_Transactional);
		Track->AddSection(*NewSection);
		Section =  CastChecked<UMovieSceneParticleSection>(NewSection);
	}
	Section->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;

	TMovieSceneChannelData<uint8> Data = Section->ParticleKeys.GetData();
	Data.Reset();
	bool LastState = Actor->GetRootComponent()->bAutoActivate;

	if (ToggleTrack.Num() > 0 && !FMath::IsNearlyZero(ToggleTrack[0].Time) )
	{
		if (ToggleTrack[0].ToggleAction == ETrackToggleAction::ETTA_Toggle)
		{
			ToggleTrack.Insert({0,LastState? ETrackToggleAction::ETTA_Off:ETrackToggleAction::ETTA_On},0);
		}
		else if (ToggleTrack[0].ToggleAction == ETrackToggleAction::ETTA_On && !LastState)
		{
			ToggleTrack.Insert({0,ETrackToggleAction::ETTA_Off},0);
		}
		else if (ToggleTrack[0].ToggleAction == ETrackToggleAction::ETTA_Off && LastState)
		{
			ToggleTrack.Insert({0,ETrackToggleAction::ETTA_On},0);
		}
	}
	
	for (const FLegacyToggleTrackKey& Key :ToggleTrack)
	{
		switch (Key.ToggleAction)
		{
		case ETrackToggleAction::ETTA_Off:
			Data.AddKey(Sequencer->GetRootTickResolution().AsFrameNumber(Key.Time),static_cast<uint8>(EParticleKey::Deactivate));
			LastState = false;
			break;
		case ETrackToggleAction::ETTA_On:
			Data.AddKey(Sequencer->GetRootTickResolution().AsFrameNumber(Key.Time),static_cast<uint8>(EParticleKey::Activate));
			LastState = true;
			break;
		case ETrackToggleAction::ETTA_Toggle:
			LastState = !LastState;
			Data.AddKey(Sequencer->GetRootTickResolution().AsFrameNumber(Key.Time),static_cast<uint8>(LastState?EParticleKey::Activate:EParticleKey::Deactivate));
			break;
		case ETrackToggleAction::ETTA_Trigger:
			LastState = !LastState;
			Data.AddKey(Sequencer->GetRootTickResolution().AsFrameNumber(Key.Time),static_cast<uint8>(EParticleKey::Trigger));
			break;
		default: ;
		}
	}
	
	Section->SetRange(TRange<FFrameNumber>::All());
	

	
	Super::ExportToLevelSequence(Sequencer, LegacyAction);
}

void ULegacyInterpTrackVisibility::ExportToLevelSequence(const TSharedRef<ISequencer>& Sequencer, ULegacyActor* LegacyAction)
{
	if (!ensure(LegacyAction))
	{
		return;
	}
	
	AActor * Actor = Cast<AActor>(LegacyAction->PresentObject);
	if (!Actor)
	{
		return;
	}
	ULegacyInterpGroup* InterpGroup = GetTypedOuter<ULegacyInterpGroup>();
	if (!ensure(InterpGroup))
	{
		return;
	}
	
	ULegacyInterpData* InterpData = GetTypedOuter<ULegacyInterpData>();
	if (!ensure(InterpData))
	{
		return;
	}
	
	
	FGuid ObjectGuid = InterpData->FindOrCreateBinding(*Actor,InterpGroup->GroupName.ToString());
	UMovieSceneVisibilityTrack* Track = InterpData->FindOrCreateTrack<UMovieSceneVisibilityTrack>(ObjectGuid);
	FName PropertyName =  AActor::GetHiddenPropertyName();
	Track->SetPropertyNameAndPath( PropertyName, PropertyName.ToString());
	
	bool bSectionAdded = false;
	UMovieSceneVisibilitySection* Section = CastChecked<UMovieSceneVisibilitySection>(Track->FindOrAddSection(0, bSectionAdded));
	Section->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;
	
	TMovieSceneChannelData<bool> Data = Section->GetChannel().GetData();
	Data.Reset();
	bool LastState = !Actor->IsHidden();

	if (VisibilityTrack.Num() > 0 && !FMath::IsNearlyZero(VisibilityTrack[0].Time) )
	{
		if (VisibilityTrack[0].Action == EVisibilityTrackAction::EVTA_Toggle)
		{
			VisibilityTrack.Insert({0,LastState? EVisibilityTrackAction::EVTA_Hide:EVisibilityTrackAction::EVTA_Show},0);
		}
		else if (VisibilityTrack[0].Action == EVisibilityTrackAction::EVTA_Show && !LastState)
		{
			VisibilityTrack.Insert({0,EVisibilityTrackAction::EVTA_Hide},0);
		}
		else if (VisibilityTrack[0].Action == EVisibilityTrackAction::EVTA_Hide && LastState)
		{
			VisibilityTrack.Insert({0,EVisibilityTrackAction::EVTA_Show},0);
		}
	}
	
	for (const FLegacyVisibilityTrackKey& Key :VisibilityTrack)
	{
		switch (Key.Action)
		{
		case EVisibilityTrackAction::EVTA_Hide:
			Data.AddKey(Sequencer->GetRootTickResolution().AsFrameNumber(Key.Time),false);
			LastState = false;
			break;
		case EVisibilityTrackAction::EVTA_Show:
			Data.AddKey(Sequencer->GetRootTickResolution().AsFrameNumber(Key.Time),true);
			LastState = true;
			break;
		case EVisibilityTrackAction::EVTA_Toggle:
			LastState = !LastState;
			Data.AddKey(Sequencer->GetRootTickResolution().AsFrameNumber(Key.Time),LastState);
			break;
		default: ;
		}
	}
	
	Section->SetRange(TRange<FFrameNumber>::All());
	

	
	Super::ExportToLevelSequence(Sequencer, LegacyAction);
}

void ULegacyInterpTrackAnimControl::ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer, ULegacyActor* LegacyAction)
{
	if (!ensure(LegacyAction))
	{
		return;
	}
	
	AActor * Actor = Cast<AActor>(LegacyAction->PresentObject);
	if (!Actor)
	{
		return;
	}

	
	USkeletalMeshComponent* SkeletalMeshComponent = Actor->FindComponentByClass<USkeletalMeshComponent>();
	if (!SkeletalMeshComponent)
	{
		return;
	}
	
	if (!SkeletalMeshComponent->GetSkinnedAsset())
	{
		return;
	}
	
	USkeleton *Skeleton = SkeletalMeshComponent->GetSkinnedAsset()->GetSkeleton();
	if (!Skeleton)
	{
		return;
	}
	
	ULegacyInterpGroup* InterpGroup = GetTypedOuter<ULegacyInterpGroup>();
	if (!ensure(InterpGroup))
	{
		return;
	}
	
	ULegacyInterpData* InterpData = GetTypedOuter<ULegacyInterpData>();
	if (!ensure(InterpData))
	{
		return;
	}
	
	FFrameRate DisplayRate = Sequencer->GetRootDisplayRate();
	FFrameRate TickResolution = Sequencer->GetRootTickResolution();

	
	FGuid ObjectGuid = InterpData->FindOrCreateBinding(*SkeletalMeshComponent,InterpGroup->GroupName.ToString());
	UMovieSceneSkeletalAnimationTrack* Track = InterpData->FindOrCreateTrack<UMovieSceneSkeletalAnimationTrack>(ObjectGuid);
	

	for (FLegacyAnimControlTrackKey&Seq :AnimSeqs)
	{
		if (UAnimSequence* AnimSequence = InterpGroup->FindAnimSequence(Seq.AnimSeqName,Skeleton))
		{
			ensure (!Seq.bLooping);
			UMovieSceneSkeletalAnimationSection* AnimationSection = CastChecked<UMovieSceneSkeletalAnimationSection>(Track->AddNewAnimation(Sequencer->GetRootTickResolution().AsFrameNumber(Seq.StartTime),AnimSequence));
			
			AnimationSection->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;
			AnimationSection->Params.PlayRate = Seq.AnimPlayRate;
			AnimationSection->Params.bReverse = Seq.bReverse;
			AnimationSection->Params.StartFrameOffset = ConvertFrameTime(FFrameTime::FromDecimal(DisplayRate.AsDecimal() * Seq.AnimStartOffset), DisplayRate, TickResolution).FrameNumber;
			AnimationSection->Params.EndFrameOffset = ConvertFrameTime(FFrameTime::FromDecimal(DisplayRate.AsDecimal() * Seq.AnimEndOffset), DisplayRate, TickResolution).FrameNumber;
		}
	
	}
}
void ULegacyInterpTrackMove::ExportToLevelSequence(const TSharedRef<ISequencer>& Sequencer, ULegacyActor* LegacyAction)
{
	if (!ensure(LegacyAction))
	{
		return;
	}
	
	AActor * Actor = Cast<AActor>(LegacyAction->PresentObject);
	if (!Actor)
	{
		return;
	}
	USceneComponent* SceneComponent = Actor->GetRootComponent();
	if (!SceneComponent)
	{
		return;
	}
	
	ULegacyInterpGroup* InterpGroup = GetTypedOuter<ULegacyInterpGroup>();
	if (!ensure(InterpGroup))
	{
		return;
	}
	
	ULegacyInterpData* InterpData = GetTypedOuter<ULegacyInterpData>();
	if (!ensure(InterpData))
	{
		return;
	}

	FGuid ObjectGuid = InterpData->FindOrCreateBinding(*SceneComponent,InterpGroup->GroupName.ToString());
	UMovieScene3DTransformTrack* MovieSceneTrack = InterpData->FindOrCreateTrack<UMovieScene3DTransformTrack>(ObjectGuid);
	FTransform3f ComponentRotationTransform( FQuat4f(SceneComponent->GetComponentQuat()));
	FTransform3f RotationTransform(	LegacyAction->Rotation);

	TArray<FFrameNumber> LocationFrameNumbers;
	TArray<FFrameNumber> RotationFrameNumbers;
	TArray<FFrameNumber> ScaleFrameNumbers;

	TArray<FMovieSceneDoubleValue> LocationXValues;
	TArray<FMovieSceneDoubleValue> LocationYValues;
	TArray<FMovieSceneDoubleValue> LocationZValues;

	TArray<FMovieSceneDoubleValue> RotationXValues;
	TArray<FMovieSceneDoubleValue> RotationYValues;
	TArray<FMovieSceneDoubleValue> RotationZValues;

	TArray<FMovieSceneDoubleValue> ScaleXValues;
	TArray<FMovieSceneDoubleValue> ScaleYValues;
	TArray<FMovieSceneDoubleValue> ScaleZValues;

	
	LocationFrameNumbers.Reserve(PosTrack.Points.Num());
	LocationXValues.Reserve(PosTrack.Points.Num());
	LocationYValues.Reserve(PosTrack.Points.Num());
	LocationZValues.Reserve(PosTrack.Points.Num());

	RotationFrameNumbers.Reserve(EulerTrack.Points.Num());
	RotationXValues.Reserve(EulerTrack.Points.Num());
	RotationYValues.Reserve(EulerTrack.Points.Num());
	RotationZValues.Reserve(EulerTrack.Points.Num());

	ScaleFrameNumbers.Reserve(0);
	ScaleXValues.Reserve(0);
	ScaleYValues.Reserve(0);
	ScaleZValues.Reserve(0);

	FFrameRate FrameRate =  Sequencer->GetRootMovieSceneSequence()->GetMovieScene()->GetTickResolution();
	
	auto SetSceneValue = [FrameRate](FMovieSceneDoubleValue& InSceneValue,EInterpCurveMode CurveMode,float ArriveTangent,float LeaveTangent)
	{
		InSceneValue.InterpMode = RCIM_Cubic;
		InSceneValue.TangentMode = RCTM_Auto;
		 switch (CurveMode)
		 {
		 case CIM_Linear:
		 	InSceneValue.InterpMode = RCIM_Linear;
		 	InSceneValue.TangentMode = RCTM_None;
		 	break;
		 case CIM_CurveAuto:
		 	InSceneValue.InterpMode = RCIM_Cubic;
		 	InSceneValue.TangentMode = RCTM_Auto;
		 	break;
		 case CIM_Constant:
		 	InSceneValue.InterpMode = RCIM_Constant;
		 	InSceneValue.TangentMode = RCTM_None;
		 	break;
		 case CIM_CurveUser:
		 	InSceneValue.InterpMode = RCIM_Cubic;
		 	InSceneValue.TangentMode = RCTM_User;
		 	break;
		 case CIM_CurveBreak:
		 	InSceneValue.InterpMode = RCIM_Cubic;
		 	InSceneValue.TangentMode = RCTM_Break;
		 	break;
		 case CIM_CurveAutoClamped:
		 	InSceneValue.InterpMode = RCIM_Cubic;
		 	InSceneValue.TangentMode = RCTM_Auto;
		 	break;
		 default: 
		 	InSceneValue.InterpMode = RCIM_Cubic;
		 	InSceneValue.TangentMode = RCTM_Auto;
		 }
		InSceneValue.Tangent.ArriveTangent = ArriveTangent/FrameRate.AsDecimal();
		InSceneValue.Tangent.LeaveTangent = LeaveTangent/FrameRate.AsDecimal();
	};

	

	for (int32 i = 0; i < PosTrack.Points.Num() ; i++)
	{
		FFrameNumber Time = Sequencer->GetRootTickResolution().AsFrameNumber(PosTrack.Points[i].InVal);
		FVector3f Position = PosTrack.Points[i].OutVal;
		
		FVector3f LeaveTangent = PosTrack.Points[i].LeaveTangent;
		FVector3f ArriveTangent = PosTrack.Points[i].ArriveTangent;
		if (MoveFrame == ELegacyInterpTrackMoveFrame::IMF_RelativeToInitial)
		{
			Position = RotationTransform.TransformPosition(Position);
			LeaveTangent =  RotationTransform.TransformPosition(LeaveTangent);
			ArriveTangent =  RotationTransform.TransformPosition(ArriveTangent);
		}
		SetSceneValue(LocationXValues.Emplace_GetRef(Position.X),PosTrack.Points[i].InterpMode,ArriveTangent.X,LeaveTangent.X);
		SetSceneValue(LocationYValues.Emplace_GetRef(Position.Y),PosTrack.Points[i].InterpMode,ArriveTangent.Y,LeaveTangent.Y);
		SetSceneValue(LocationZValues.Emplace_GetRef(Position.Z),PosTrack.Points[i].InterpMode,ArriveTangent.Z,LeaveTangent.Z);
		LocationFrameNumbers.Add(Time);
	}
	
	for (int32 i = 0; i < EulerTrack.Points.Num() ; i++)
	{
		
		FRotator3f LeaveTangent = {EulerTrack.Points[i].LeaveTangent.Y,EulerTrack.Points[i].LeaveTangent.Z,EulerTrack.Points[i].LeaveTangent.X};
		FRotator3f ArriveTangent = {EulerTrack.Points[i].ArriveTangent.Y,EulerTrack.Points[i].ArriveTangent.Z,EulerTrack.Points[i].ArriveTangent.X};
		FRotator3f Rotator = {EulerTrack.Points[i].OutVal.Y,EulerTrack.Points[i].OutVal.Z,EulerTrack.Points[i].OutVal.X};
		if (MoveFrame == ELegacyInterpTrackMoveFrame::IMF_RelativeToInitial)
		{
			Rotator = RotationTransform.TransformRotation( Rotator.Quaternion()).Rotator();
			Rotator -= FRotator3f(Actor->GetActorRotation());
			LeaveTangent = RotationTransform.TransformRotation( LeaveTangent.Quaternion()).Rotator();
			LeaveTangent -= FRotator3f(Actor->GetActorRotation());
			ArriveTangent = RotationTransform.TransformRotation( ArriveTangent.Quaternion()).Rotator();
			ArriveTangent -= FRotator3f(Actor->GetActorRotation());
		}
		FFrameNumber Time = Sequencer->GetRootTickResolution().AsFrameNumber(EulerTrack.Points[i].InVal);
		SetSceneValue(RotationXValues.Emplace_GetRef(Rotator.Roll),EulerTrack.Points[i].InterpMode,ArriveTangent.Roll,LeaveTangent.Roll);
		SetSceneValue(RotationYValues.Emplace_GetRef(Rotator.Pitch),EulerTrack.Points[i].InterpMode,ArriveTangent.Pitch,LeaveTangent.Pitch);
		SetSceneValue(RotationZValues.Emplace_GetRef(Rotator.Yaw),EulerTrack.Points[i].InterpMode,ArriveTangent.Yaw,LeaveTangent.Yaw);
		RotationFrameNumbers.Add(Time);
	}
	
	bool bSectionAdded = false;
	UMovieScene3DTransformSection* Section = Cast<UMovieScene3DTransformSection>(MovieSceneTrack->FindOrAddSection(0, bSectionAdded));
	Section->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;
	
	if (MoveFrame == ELegacyInterpTrackMoveFrame::IMF_RelativeToInitial)
	{
		Section->SetBlendType(EMovieSceneBlendType::Relative);
	}
	else
	{
		Section->SetBlendType(EMovieSceneBlendType::Absolute);
	}
	
	Section->SetRange(TRange<FFrameNumber>::All());

	TArrayView<FMovieSceneDoubleChannel*> Channels = Section->GetChannelProxy().GetChannels<FMovieSceneDoubleChannel>();
	if (!ensure(Channels.Num() >= 9))
	{
		return;
	}

	Channels[0]->Set(LocationFrameNumbers, LocationXValues);
	Channels[1]->Set(LocationFrameNumbers, LocationYValues);
	Channels[2]->Set(LocationFrameNumbers, LocationZValues);

	Channels[3]->Set(RotationFrameNumbers, RotationXValues);
	Channels[4]->Set(RotationFrameNumbers, RotationYValues);
	Channels[5]->Set(RotationFrameNumbers, RotationZValues);

	Channels[6]->Set(ScaleFrameNumbers, ScaleXValues);
	Channels[7]->Set(ScaleFrameNumbers, ScaleYValues);
	Channels[8]->Set(ScaleFrameNumbers, ScaleZValues);
}

void ULegacyInterpTrackFloatMaterialParam::ExportToLevelSequence(const TSharedRef<ISequencer>& Sequencer, ULegacyActor* LegacyAction)
{
	if (!ensure(LegacyAction))
	{
		return;
	}
	
	AMaterialInstanceHybridActor * MaterialInstanceHybridActor = Cast<AMaterialInstanceHybridActor>(LegacyAction->PresentObject);
	if (!MaterialInstanceHybridActor)
	{
		return;
	}

	ULegacyInterpGroup* InterpGroup = GetTypedOuter<ULegacyInterpGroup>();
	if (!ensure(InterpGroup))
	{
		return;
	}
	
	ULegacyInterpData* InterpData = GetTypedOuter<ULegacyInterpData>();
	if (!ensure(InterpData))
	{
		return;
	}

	if (!MaterialInstanceHybridActor->MaterialInstance)
	{
		return;
	}

	FFrameRate FrameRate =  Sequencer->GetRootMovieSceneSequence()->GetMovieScene()->GetTickResolution();
	
	auto SetSceneValue = [FrameRate](FMovieSceneFloatValue& InSceneValue,EInterpCurveMode CurveMode,float ArriveTangent,float LeaveTangent)
	{
		InSceneValue.InterpMode = RCIM_Cubic;
		InSceneValue.TangentMode = RCTM_Auto;
		switch (CurveMode)
		{
		case CIM_Linear:
			InSceneValue.InterpMode = RCIM_Linear;
			InSceneValue.TangentMode = RCTM_None;
			break;
		case CIM_CurveAuto:
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_Auto;
			break;
		case CIM_Constant:
			InSceneValue.InterpMode = RCIM_Constant;
			InSceneValue.TangentMode = RCTM_None;
			break;
		case CIM_CurveUser:
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_User;
			break;
		case CIM_CurveBreak:
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_Break;
			break;
		case CIM_CurveAutoClamped:
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_Auto;
			break;
		default: 
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_Auto;
		}
		InSceneValue.Tangent.ArriveTangent = ArriveTangent/FrameRate.AsDecimal();
		InSceneValue.Tangent.LeaveTangent = LeaveTangent/FrameRate.AsDecimal();
	};
		
	FGuid ObjectGuid = InterpData->FindOrCreateBinding(*MaterialInstanceHybridActor,InterpGroup->GroupName.ToString());
	UMovieSceneHybridMaterialTrack* Track = InterpData->FindOrCreateTrack<UMovieSceneHybridMaterialTrack>(ObjectGuid);

	
	Track->SetMaterialHybrid(MaterialInstanceHybridActor->MaterialInstance);
	
	UMovieSceneHybridMaterialParameterSection* Section = CastChecked<UMovieSceneHybridMaterialParameterSection>(Track->CreateNewSection());

	Section->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;
	if (Section)
	{
		Track->Modify();
		Track->AddSection(*Section);
	}
	
	
	FMovieSceneFloatChannel*FloatChannel =  Section->FindOrAddScalarParameterKey(ParamName);
	check(FloatChannel);
	Section->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;
	TArray<FFrameNumber> ScalarFrameNumbers;
	TArray<FMovieSceneFloatValue> ScalarValues;

	
	ScalarFrameNumbers.Reserve(FloatTrack.Points.Num());
	ScalarValues.Reserve(FloatTrack.Points.Num());

	for (const FInterpCurvePoint<float>& Key :FloatTrack.Points)
	{
		FFrameNumber Time = Sequencer->GetRootTickResolution().AsFrameNumber(Key.InVal);
		SetSceneValue(ScalarValues.Emplace_GetRef(Key.OutVal),Key.InterpMode,Key.ArriveTangent,Key.LeaveTangent);
		ScalarFrameNumbers.Add(Time);
		
	}
	Section->SetRange(TRange<FFrameNumber>::All());
	FloatChannel->Set(ScalarFrameNumbers,ScalarValues);

	
	Super::ExportToLevelSequence(Sequencer, LegacyAction);
	
}

void ULegacyInterpTrackFloatParticleParam::ExportToLevelSequence(const TSharedRef<ISequencer>& Sequencer, ULegacyActor* LegacyAction)
{
	if (!ensure(LegacyAction))
	{
		return;
	}
	
	AEmitter * Emitter = Cast<AEmitter>(LegacyAction->PresentObject);
	if (!Emitter)
	{
		return;
	}

	ULegacyInterpGroup* InterpGroup = GetTypedOuter<ULegacyInterpGroup>();
	if (!ensure(InterpGroup))
	{
		return;
	}
	
	ULegacyInterpData* InterpData = GetTypedOuter<ULegacyInterpData>();
	if (!ensure(InterpData))
	{
		return;
	}

	FFrameRate FrameRate =  Sequencer->GetRootMovieSceneSequence()->GetMovieScene()->GetTickResolution();
	
	auto SetSceneValue = [FrameRate](FMovieSceneFloatValue& InSceneValue,EInterpCurveMode CurveMode,float ArriveTangent,float LeaveTangent)
	{
		InSceneValue.InterpMode = RCIM_Cubic;
		InSceneValue.TangentMode = RCTM_Auto;
		switch (CurveMode)
		{
		case CIM_Linear:
			InSceneValue.InterpMode = RCIM_Linear;
			InSceneValue.TangentMode = RCTM_None;
			break;
		case CIM_CurveAuto:
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_Auto;
			break;
		case CIM_Constant:
			InSceneValue.InterpMode = RCIM_Constant;
			InSceneValue.TangentMode = RCTM_None;
			break;
		case CIM_CurveUser:
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_User;
			break;
		case CIM_CurveBreak:
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_Break;
			break;
		case CIM_CurveAutoClamped:
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_Auto;
			break;
		default: 
			InSceneValue.InterpMode = RCIM_Cubic;
			InSceneValue.TangentMode = RCTM_Auto;
		}
		InSceneValue.Tangent.ArriveTangent = ArriveTangent/FrameRate.AsDecimal();
		InSceneValue.Tangent.LeaveTangent = LeaveTangent/FrameRate.AsDecimal();
	};

	if (FloatTrack.Points.Num() == 0)
	{
		return;
	}
	
	FGuid ObjectGuid = InterpData->FindOrCreateBinding(*Emitter,InterpGroup->GroupName.ToString());
	UMovieSceneParticleParameterTrack* Track = InterpData->FindOrCreateTrack<UMovieSceneParticleParameterTrack>(ObjectGuid);
	
	UMovieSceneParameterSection* Section = CastChecked<UMovieSceneParameterSection>(Track->CreateNewSection());
	Section->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;

	if (Section)
	{
		Track->Modify();
		Track->AddSection(*Section);
	}
	Section->AddScalarParameterKey(ParamName,Sequencer->GetRootTickResolution().AsFrameNumber(FloatTrack.Points[0].InVal),FloatTrack.Points[0].OutVal);
	
	FMovieSceneFloatChannel*FloatChannel = nullptr;
	{
		for ( FScalarParameterNameAndCurve& ScalarParameterNameAndCurve : Section->GetScalarParameterNamesAndCurves() )
		{
			if ( ScalarParameterNameAndCurve.ParameterName == ParamName )
			{
				FloatChannel = &ScalarParameterNameAndCurve.ParameterCurve;
				break;
			}
		}
	}
	
	check(FloatChannel);
	
	Section->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;
	TArray<FFrameNumber> ScalarFrameNumbers;
	TArray<FMovieSceneFloatValue> ScalarValues;

	
	ScalarFrameNumbers.Reserve(FloatTrack.Points.Num());
	ScalarValues.Reserve(FloatTrack.Points.Num());

	for (const FInterpCurvePoint<float>& Key :FloatTrack.Points)
	{
		FFrameNumber Time = Sequencer->GetRootTickResolution().AsFrameNumber(Key.InVal);
		SetSceneValue(ScalarValues.Emplace_GetRef(Key.OutVal),Key.InterpMode,Key.ArriveTangent,Key.LeaveTangent);
		ScalarFrameNumbers.Add(Time);
		
	}
	Section->SetRange(TRange<FFrameNumber>::All());
	FloatChannel->Set(ScalarFrameNumbers,ScalarValues);

	
}

void ULegacyXAKInterpTrackPostEvent::ExportToLevelSequence(const TSharedRef<ISequencer>& Sequencer, ULegacyActor* LegacyAction)
{
	if (!ensure(LegacyAction))
	{
		return;
	}
	
	AActor * Actor = Cast<AActor>(LegacyAction->PresentObject);
	if (!Actor)
	{
		return;
	}

	ULegacyInterpGroup* InterpGroup = GetTypedOuter<ULegacyInterpGroup>();
	if (!ensure(InterpGroup))
	{
		return;
	}
	
	ULegacyInterpData* InterpData = GetTypedOuter<ULegacyInterpData>();
	if (!ensure(InterpData))
	{
		return;
	}

	FFrameRate FrameRate =  Sequencer->GetRootMovieSceneSequence()->GetMovieScene()->GetTickResolution();
	
	if (AKEvents.Num() == 0)
	{
		return;
	}
	
	FGuid ObjectGuid = InterpData->FindOrCreateBinding(*Actor,InterpGroup->GroupName.ToString());
	UMovieSceneAudioTrack* Track = InterpData->FindOrCreateTrack<UMovieSceneAudioTrack>(ObjectGuid);
	
	for (FLegacyXAKEventTrackKey&Key : AKEvents)
	{
		if (Key.Event)
		{
			if (USoundBase* Sound = Cast<USoundBase>(Key.Event->ExportToContent()))
			{
				FFrameNumber Time = Sequencer->GetRootTickResolution().AsFrameNumber(Key.Time);
				if (UMovieSceneSection* MovieSceneSection = Track->AddNewSound(Sound,Time))
				{
					MovieSceneSection->EvalOptions.CompletionMode = EMovieSceneCompletionMode::KeepState;
					MovieSceneSection->Modify();
				}
			}
		}
	}
	

}

void ULegacyInterpGroup::ExportToLevelSequence(ULegacySeqAct_Interp* OwnerSeqAction, const TSharedRef<ISequencer>&Sequencer)
{
	for (FLegacySeqVarLink& VarLink : OwnerSeqAction->VariableLinks)
	{
		FName LinkName = VarLink.XLinkName;
		if (LinkName == NAME_None)
		{
			LinkName = *VarLink.LinkDesc;
		}
		if (LinkName == GroupName)
		{
			if (VarLink.LinkedVariables.Num() > 0)
			{
				for (ULegacySequenceVariable* Variable : VarLink.LinkedVariables)
				{
					ULegacySeqVar_Object* Var_Object = Cast<ULegacySeqVar_Object>(Variable);
					if (ULegacySeqVar_Named* Var_Named = Cast<ULegacySeqVar_Named>(Variable))
					{
						Var_Object = Cast<ULegacySeqVar_Object>(Var_Named->FindVariable());
					}
					if (Var_Object)
					{
						if (ULegacyActor* LegacyActor = Cast<ULegacyActor>(Var_Object->ObjValue) )
						{
							for (ULegacyInterpTrack* InterpTrack :InterpTracks)
							{
								if (InterpTrack)
								{
									InterpTrack->ExportToLevelSequence(Sequencer,LegacyActor);
								}
							}
						}
					}
					else if (ULegacySeqVar_ObjectList* Var_ObjectList = Cast<ULegacySeqVar_ObjectList>(Variable))
					{
						for (ULegacyObject* Object:Var_ObjectList->ObjList)
						{
							if (ULegacyActor* LegacyActor = Cast<ULegacyActor>(Object) )
							{
								for (ULegacyInterpTrack* InterpTrack :InterpTracks)
								{
									if (InterpTrack)
									{
										InterpTrack->ExportToLevelSequence(Sequencer,LegacyActor);
									}
								}
							}
						}
					}
				}
				return;
			}
		}
	}
	for (ULegacyInterpTrack* InterpTrack :InterpTracks)
	{
		if (InterpTrack)
		{
			InterpTrack->ExportToLevelSequence(Sequencer,nullptr);
		}
	}
}

UAnimSequence* ULegacyInterpGroup::FindAnimSequence(const FName& InName, USkeleton* Skeleton)
{
	for (ULegacyAnimSet* AnimSet:	GroupAnimSets)
	{
		for (ULegacyAnimSequence* AnimSeq:	AnimSet->Sequences)
		{
			if (AnimSeq->SequenceName == InName)
			{
				return AnimSeq->CreateSequence(AnimSet,Skeleton);
			}
		}
	}
	return nullptr;
}

FName ULegacyInterpData::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(!VarGuid.IsValid())
	{
		if(VarName == NAME_None)
		{
			VarName = GetLegacyFName();
		}
		FEdGraphPinType ObjectPinType(UEdGraphSchema_K2::PC_Object, NAME_None, ALevelSequenceActor::StaticClass(), EPinContainerType::None, false, FEdGraphTerminalType());
		if(ensure(FBlueprintEditorUtils::AddMemberVariable(InBlueprint, VarName, ObjectPinType)))
		{
			InBlueprint->NewVariables.Last().PropertyFlags &= ~CPF_DisableEditOnInstance;
			if(ensure(InBlueprint->NewVariables.Last().VarName == VarName))
			{
				VarGuid = InBlueprint->NewVariables.Last().VarGuid;
			}
		}
		if(!VarGuid.IsValid())
		{
			return NAME_None;
		}
	}
	FBPVariableDescription* PointerPtr = InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });
	if (!PointerPtr)
	{
		return NAME_None;
	}
	return PointerPtr->VarName;
}

void ULegacyInterpData::Fill(ALegacyKismet* Kismet)
{
	CurrentKismet = Kismet;
	if(VarName == NAME_None)
	{
		VarName = GetLegacyFName();
	}
	if (FProperty* Property = Kismet->GetClass()->FindPropertyByName(VarName))
	{
		ALevelSequenceActor* LevelSequenceActor = GWorld->SpawnActor<ALevelSequenceActor>();
		LevelSequenceActor->SetSequence(GetOrCreateLevel());
		Property->SetValue_InContainer(Kismet,&LevelSequenceActor);
	}
	CurrentKismet = nullptr;
}

ULevelSequence* ULegacyInterpData::GetOrCreateLevel()
{
	if(LevelSequence)
	{
		return LevelSequence;
	}

	auto ReCreateObject = [](const FString& Path,UObject*&OutNewObject,UClass* InClass,EObjectFlags InFlags)
	{
		UObject* ExistingAsset = nullptr;
		
		UPackage* InParent = CreatePackage(*Path);
		FName InName = *FPaths::GetBaseFilename(Path);
		// create an asset if it doesn't exist
		ExistingAsset = StaticFindObject(nullptr, InParent, *InName.ToString());
		
		if (!ExistingAsset)
		{
			OutNewObject = NewObject<UObject>(InParent, InClass, InName, InFlags, nullptr);
			ensure(OutNewObject);
			return true;
		}
		
		// otherwise delete and replace
		if (!ObjectTools::ForceDeleteObjects({ExistingAsset},false))
		{
			UE_LOG(LogRedUELegacy, Warning, TEXT("Could not delete existing asset %s"), *ExistingAsset->GetFullName());
			OutNewObject = nullptr;
			return false;
		}

		InParent = CreatePackage(*Path);
		// try to find the existing asset again now that the GC has occurred
		ExistingAsset = StaticFindObject(nullptr, InParent, *InName.ToString());

		// if the object is still around after GC, fail this operation
		if (ExistingAsset)
		{

			OutNewObject = nullptr;
			return false;
		}
		// create the asset in the package
		OutNewObject = NewObject<UObject>(InParent, InClass, InName, InFlags, nullptr);
		ensure(OutNewObject);
		return true;

	};
	const FString Name = FPaths::GetBaseFilename( LegacyPackage->FileName)/GetLegacyName();
	const FString ObjectPath = GetOutContentPath() / Name.Replace(TEXT("."),TEXT("/"));
	const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
	const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
	UObject* InNewObject = nullptr;
	if( ReCreateObject(ObjectPath,InNewObject,ULevelSequence::StaticClass(),RF_Public | RF_Standalone | RF_Transactional))
	{
		LevelSequence = CastChecked<ULevelSequence>(InNewObject);

		FAssetRegistryModule::AssetCreated(LevelSequence);
		LevelSequence->Initialize();
		LevelSequence->PreEditChange(nullptr);

		FFrameRate TickResolution = LevelSequence->GetMovieScene()->GetTickResolution();
		LevelSequence->GetMovieScene()->SetPlaybackRange((0.f*TickResolution).FloorToFrame(), (InterpLength*TickResolution).FloorToFrame().Value);

		ExportToLevelSequence(OwnerSeqAct_Interp,LevelSequence);
		
		LevelSequence->PostEditChange();
		LevelSequence->Modify();
	}
	return LevelSequence;
}

void ULegacyInterpData::ExportToLevelSequence(ULegacySeqAct_Interp* OwnerSeqAction, ULevelSequence* InLevelSequence)
{
	FSequencerInitParams SequencerInitParams;
	{
		SequencerInitParams.RootSequence = InLevelSequence;
		SequencerInitParams.ViewParams.bReadOnly = false;
		SequencerInitParams.bEditWithinLevelEditor = false;
		SequencerInitParams.ViewParams.UniqueName = "EmbeddedActorSequenceEditor";
		SequencerInitParams.ViewParams.ScrubberStyle = ESequencerScrubberStyle::FrameBlock;
	}

	const TSharedRef<ISequencer> Sequencer = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer").CreateSequencer(SequencerInitParams);
	
	for (ULegacyInterpGroup* InterpGroup :InterpGroups)
	{
		if (InterpGroup)
		{
			InterpGroup->ExportToLevelSequence(OwnerSeqAction, Sequencer);
		}
	}
	Sequencer->Close();
}

FGuid ULegacyInterpData::FindOrCreateBinding(AActor& ActorToBind, const FString& NameBinding)
{
	UMovieScene* MovieScene = LevelSequence->GetMovieScene();
	if (!MovieScene)
	{
		return {};
	}


	FGuid ActorBinding = LevelSequence->FindBindingFromObject(&ActorToBind, ActorToBind.GetWorld());
	if (!ActorBinding.IsValid())
	{
		// We use the label here because that will always be named after the prim that caused the actor
		// to be generated. If we just used our own PrimName in here we may run into situations where a child Camera prim
		// of a decomposed camera ends up naming the actor binding after itself, even though the parent Xform prim, and the
		// actor on the level, maybe named something else
		ActorBinding = MovieScene->AddPossessable(
#if WITH_EDITOR
			NameBinding,
#else
			NameBinding,
#endif	  // WITH_EDITOR
			ActorToBind.GetClass()
		);
		LevelSequence->BindPossessableObject(ActorBinding, ActorToBind, ActorToBind.GetWorld());
	}
	return ActorBinding;
}

FGuid ULegacyInterpData::FindOrCreateBinding(USceneComponent& ComponentToBind, const FString& NameBinding)
{
	UMovieScene* MovieScene = LevelSequence->GetMovieScene();
	if (!MovieScene)
	{
		return {};
	}

	if (FGuid* ExistingGuid = BindingMap.Find(&ComponentToBind))
	{
		return *ExistingGuid;
	}

	FGuid ActorBinding;
	UObject* ComponentContext = ComponentToBind.GetWorld();

	

	// Make sure we always bind the parent actor too
	if (AActor* Actor = ComponentToBind.GetOwner())
	{
		ActorBinding = LevelSequence->FindBindingFromObject(Actor, Actor->GetWorld());
		if (!ActorBinding.IsValid())
		{
			// We use the label here because that will always be named after the prim that caused the actor
			// to be generated. If we just used our own PrimName in here we may run into situations where a child Camera prim
			// of a decomposed camera ends up naming the actor binding after itself, even though the parent Xform prim, and the
			// actor on the level, maybe named something else
			ActorBinding = MovieScene->AddPossessable(
#if WITH_EDITOR
				NameBinding,
#else
				NameBinding,
#endif	  // WITH_EDITOR
				Actor->GetClass()
			);
			LevelSequence->BindPossessableObject(ActorBinding, *Actor, Actor->GetWorld());
		}

		ComponentContext = Actor;
	}

	FGuid ComponentBinding = MovieScene->AddPossessable(ComponentToBind.GetName(), ComponentToBind.GetClass());

	if (ActorBinding.IsValid() && ComponentBinding.IsValid())
	{
		if (FMovieScenePossessable* ComponentPossessable = MovieScene->FindPossessable(ComponentBinding))
		{
			ComponentPossessable->SetParent(ActorBinding, MovieScene);
		}
	}

	// Bind component
	LevelSequence->BindPossessableObject(ComponentBinding, ComponentToBind, ComponentContext);
	BindingMap.Emplace(&ComponentToBind, ComponentBinding);
	return ComponentBinding;
}
