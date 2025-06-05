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
#include "ObjectTools.h"
#include "PackageTools.h"
#include "SequencerUtilities.h"
#include "Animation/AnimSequence.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/Kismet/K2Node_SequenceAction.h"
#include "Blueprints/RedUEBlueprintFunctionLibrary.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacySubsystem.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Kismet/Base/SequenceAction.h"
#include "Kismet/Gameplay/SeqAct_Interp.h"
#include "Kismet/Gameplay/XSeqAct_InstancePattern.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Mesh/LegacyAnimSequence.h"
#include "Mesh/LegacyAnimSet.h"
#include "Tracks/MovieScene3DTransformTrack.h"
#include "Tracks/MovieSceneSkeletalAnimationTrack.h"
#include "World/LegacyWorld.h"
#include "World/Actors/LegacyActor.h"
#include "World/Sequences/LegacySequence.h"


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

void ULegacySequenceOp::SimulatedImport()
{
	
}


void ULegacySequenceImporter::FillPin(UBlueprint* InBlueprint, UEdGraph* InGraph, const FLegacySeqOpOutputLink& LegacySeqOpOutputLink, UEdGraphPin* OutputPin)
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

UK2Node_SequenceAction* ULegacySequenceImporter::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (CurrentNode)
	{
		return CurrentNode;
	}

	FGraphNodeCreator<UK2Node_SequenceAction> NodeCreator(*InGraph);
	UK2Node_SequenceAction* NewNode = NodeCreator.CreateNode();
	NewNode->Action = NewObject<USequenceAction>(NewNode,ToAction->GetClass(),NAME_None,RF_Transactional,ToAction);
	NewNode->bCommentBubblePinned = true;
	NewNode->OnUpdateCommentText(GetLegacyFullName());
	NodeCreator.Finalize();
	CurrentNode = NewNode;
	
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
			if (VariableLinks[i].LinkedVariables.Num() > 0)
			{
				FProperty* DstProperty = NewNode->Action->GetClass()->FindPropertyByName(*VariableName);
				if (!ensure(DstProperty))
				{
					continue;
				}
				
				for (int32  LinkID = 0;LinkID<VariableLinks[i].LinkedVariables.Num();LinkID++)
				{
					if (FArrayProperty* DstArrayProperty = CastField<FArrayProperty>(DstProperty))
					{
						if (ULegacySequenceVariable* Variable = CastChecked<ULegacySequenceVariable>(VariableLinks[i].LinkedVariables[LinkID],ECastCheckedType::NullAllowed))
						{
							if (FBPVariableDescription*VariableDescription = Variable->GetOrCreateVariable(InBlueprint,InGraph))
							{
								FProperty* BlueprintProperty = InBlueprint->SkeletonGeneratedClass->FindPropertyByName(VariableDescription->VarName);
								if (ensure(BlueprintProperty))
								{
									if (VariableLinks[i].LinkedVariables.Num() == 0 && BlueprintProperty->SameType(DstProperty) )
									{
										FMemberReference&Reference = NewNode->Action->PropertiesReference.Add(*VariableName);
										Reference.SetExternalMember(VariableDescription->VarName,InBlueprint->SkeletonGeneratedClass);
									}
									else
									{
										if (ensure(BlueprintProperty->SameType(DstProperty) || BlueprintProperty->SameType(DstArrayProperty->Inner)))
										{
											FScriptArrayHelper ArrayHelper(DstArrayProperty,DstArrayProperty->ContainerPtrToValuePtr<void>(NewNode->Action));
											FSequenceActionPropertyArrayReference& PropertyArrayReference = NewNode->Action->PropertiesArrayReference.FindOrAdd(*VariableName);
											PropertyArrayReference.ArrayElementReference.Add(ArrayHelper.Num()).SetExternalMember(VariableDescription->VarName,InBlueprint->SkeletonGeneratedClass);
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
						if (ULegacySequenceVariable* Variable = CastChecked<ULegacySequenceVariable>(VariableLinks[i].LinkedVariables[0],ECastCheckedType::NullAllowed))
						{
							if (FBPVariableDescription*VariableDescription = Variable->GetOrCreateVariable(InBlueprint,InGraph))
							{
								FProperty* BlueprintProperty = InBlueprint->SkeletonGeneratedClass->FindPropertyByName(VariableDescription->VarName);
								if (ensure(BlueprintProperty))
								{
									if (ensure(BlueprintProperty->SameType(DstProperty)))
									{
										FMemberReference&Reference = NewNode->Action->PropertiesReference.Add(*VariableName);
										Reference.SetExternalMember(VariableDescription->VarName,InBlueprint->SkeletonGeneratedClass);
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
	return nullptr;
}

void ULegacySequenceVariable::Fill(ALegacyKismet* Kismet)
{
}


FBPVariableDescription* ULegacySeqVar_Object::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if(!VarGuid.IsValid())
	{
		if(VarName == NAME_None)
		{
			VarName = GetLegacyFName();
		}
		{
			ensure(ObjValue&&ObjValue->IsA<ULegacyActor>());
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
			return nullptr;
		}
	}
	return InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });
}

void ULegacySeqVar_Object::Fill(ALegacyKismet* Kismet)
{
	if(VarName == NAME_None)
	{
		VarName = GetLegacyFName();
	}
	FProperty* InProperty = Kismet->GetClass()->FindPropertyByName(VarName);
	if (!InProperty)
	{
		return;
	}
	FObjectProperty* InPropertyProperty = CastField<FObjectProperty>(InProperty);
	if (ensure(InPropertyProperty))
	{
		if (ObjValue&&ObjValue->PresentObject)
		{
			InPropertyProperty->SetPropertyValue_InContainer(Kismet, ObjValue->PresentObject);
		}
	}
}

FBPVariableDescription* ULegacySeqVar_Named::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (ULegacySequenceVariable* Variable = FindVariable())
	{
		return Variable->GetOrCreateVariable(InBlueprint, InGraph);
	}
	return Super::GetOrCreateVariable(InBlueprint, InGraph);
}

void ULegacySeqVar_Named::Fill(ALegacyKismet* Kismet)
{
	if (ULegacySequenceVariable* Variable = FindVariable())
	{
		return Variable->Fill(Kismet);
	}
	Super::Fill(Kismet);
}

ULegacySequenceVariable* ULegacySeqVar_Named::FindVariable()
{
	if (RefVariable)
	{
		return RefVariable;
	}
	URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
	if (RedUELegacySubsystem->GLegacyWorld&&RedUELegacySubsystem->GLegacyWorld->PersistentLevel)
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

FBPVariableDescription* ULegacySeqVar_ObjectList::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
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
			return nullptr;
		}
	}
	return InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });

}

void ULegacySeqVar_ObjectList::Fill(ALegacyKismet* Kismet)
{
	if (VarName == NAME_None)
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

ULegacySeqAct_Interp::ULegacySeqAct_Interp()
{
	ToAction = CreateDefaultSubobject<USeqAct_Interp>("USeqAct_Interp");
}

UK2Node_SequenceAction* ULegacySeqAct_Interp::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
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
	UK2Node_SequenceAction*  Result = Super::ExportToBlueprint(InBlueprint, InGraph);
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
			EventName = GetLegacyName() + EventName;
			
			NewEventNode->CustomFunctionName = *EventName;
			NewEventNode->CreateNewGuid();
			NewEventNode->PostPlacedNewNode();
			NewEventNode->SetFlags(RF_Transactional);
			NewEventNode->AllocateDefaultPins();
			NewEventNode->bCommentBubblePinned = true;
			NewEventNode->NodePosY = 0;
			NewEventNode->OnUpdateCommentText(GetLegacyFullName());
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

ULegacyXSeqAct_InstancePattern::ULegacyXSeqAct_InstancePattern()
{
	ToAction = CreateDefaultSubobject<UXSeqAct_InstancePattern>("yXSeqAct_InstancePattern");
}

UK2Node_SequenceAction* ULegacyXSeqAct_InstancePattern::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	bool NeedCreateEvent = false;
	if (!CurrentNode)
	{		
		NeedCreateEvent = true;
	}	
	UK2Node_SequenceAction*  Result = Super::ExportToBlueprint(InBlueprint, InGraph);
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
			EventName = GetLegacyName() + EventName;
			
			NewEventNode->CustomFunctionName = *EventName;
			NewEventNode->CreateNewGuid();
			NewEventNode->PostPlacedNewNode();
			NewEventNode->SetFlags(RF_Transactional);
			NewEventNode->AllocateDefaultPins();
			NewEventNode->bCommentBubblePinned = true;
			NewEventNode->NodePosY = 0;
			NewEventNode->OnUpdateCommentText(GetLegacyFullName());
			UEdGraphSchema_K2::SetNodeMetaData(NewEventNode, FNodeMetadata::DefaultGraphNode);
			InGraph->AddNode(NewEventNode);
			FillPin(InBlueprint,InGraph,OutputLinks[i],NewEventNode ->GetThenPin());			
		}
		
	}
	
	return Result;
}

UEdGraphPin* ULegacyXSeqAct_InstancePattern::GetInputPin(int32 Index, UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return Super::GetInputPin(Index, InBlueprint, InGraph);
}


void ULegacyInterpTrack::ExportToLevelSequence( const TSharedRef<ISequencer>&Sequencer, ULegacyActor* LegacyAction)
{
}

void ULegacyInterpTrackAnimControl::ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer, ULegacyActor* LegacyAction)
{
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
	UMovieSceneSkeletalAnimationTrack* Track = Sequencer->GetRootMovieSceneSequence()->GetMovieScene()->AddTrack<UMovieSceneSkeletalAnimationTrack>(ObjectGuid);
	

	for (FLegacyAnimControlTrackKey&Seq :AnimSeqs)
	{
		if (UAnimSequence* AnimSequence = InterpGroup->FindAnimSequence(Seq.AnimSeqName,Skeleton))
		{
			ensure (!Seq.bLooping);
			UMovieSceneSkeletalAnimationSection* AnimationSection = CastChecked<UMovieSceneSkeletalAnimationSection>(Track->AddNewAnimation(Sequencer->GetRootTickResolution().AsFrameNumber(Seq.StartTime),AnimSequence));
			AnimationSection->Params.PlayRate = Seq.AnimPlayRate;
			AnimationSection->Params.bReverse = Seq.bReverse;
			AnimationSection->Params.StartFrameOffset = ConvertFrameTime(FFrameTime::FromDecimal(DisplayRate.AsDecimal() * Seq.AnimStartOffset), DisplayRate, TickResolution).FrameNumber;
			AnimationSection->Params.EndFrameOffset = ConvertFrameTime(FFrameTime::FromDecimal(DisplayRate.AsDecimal() * Seq.AnimEndOffset), DisplayRate, TickResolution).FrameNumber;
		}
	
	}
}

void ULegacyInterpTrackMove::ExportToLevelSequence(const TSharedRef<ISequencer>& Sequencer, ULegacyActor* LegacyAction)
{

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
	UMovieScene3DTransformTrack* MovieSceneTrack = Sequencer->GetRootMovieSceneSequence()->GetMovieScene()->AddTrack<UMovieScene3DTransformTrack>(ObjectGuid);
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
		 	InSceneValue.TangentMode = RCTM_Auto;
		 	break;
		 case CIM_CurveAuto:
		 	InSceneValue.InterpMode = RCIM_Cubic;
		 	InSceneValue.TangentMode = RCTM_Auto;
		 	break;
		 case CIM_Constant:
		 	InSceneValue.InterpMode = RCIM_Constant;
		 	InSceneValue.TangentMode = RCTM_Auto;
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
			}
			return;
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

FBPVariableDescription* ULegacyInterpData::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
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
			return nullptr;
		}
	}
	return InBlueprint->NewVariables.FindByPredicate([this](const FBPVariableDescription& Item) { return Item.VarGuid == VarGuid; });

}

void ULegacyInterpData::Fill(ALegacyKismet* Kismet)
{
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

		// overwrite existing asset, if possible
		if (ExistingAsset->GetClass()->IsChildOf(InClass))
		{
			OutNewObject = NewObject<UObject>(InParent, InClass, InName, InFlags, nullptr);
			ensure(OutNewObject);
			return true;
		}
		
		// otherwise delete and replace
		if (!ObjectTools::DeleteSingleObject(ExistingAsset))
		{
			UE_LOG(LogRedUELegacy, Warning, TEXT("Could not delete existing asset %s"), *ExistingAsset->GetFullName());
			OutNewObject = nullptr;
			return false;
		}

		// keep InPackage alive through the GC, in case ExistingAsset was the only reason it was around
		const bool bRootedPackage = InParent->IsRooted();

		if (!bRootedPackage)
		{
			InParent->AddToRoot();
		}

		// force GC so we can cleanly create a new asset (and not do an 'in place' replacement)
		CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);

		if (!bRootedPackage)
		{
			InParent->RemoveFromRoot();
		}

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
