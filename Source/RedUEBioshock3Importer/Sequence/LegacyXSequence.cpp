#include "LegacyXSequence.h"

#include "K2Node_CustomEvent.h"
#include "Blueprint/Kismet/K2Node_SequenceAction.h"
#include "Editor/Sequencer/Public/ISequencer.h"
#include "Entities/LegacyActor.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Kismet/Gameplay/XSeqAct_InstancePattern.h"
#include "Kismet/XCore/XSeqAct_PlaySound.h"
#include "Sounds/LegacyXSound.h"
#include "Tracks/MovieSceneAudioTrack.h"


FName ULegacyXSeqVar_PlayerController::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return GET_MEMBER_NAME_CHECKED(ALegacyKismet,PlayerController);
}

FName ULegacyXSeqVar_Elizabeth::GetOrCreateVariable(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	return GET_MEMBER_NAME_CHECKED(ABioshockKismet,ElizabethPawn);
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