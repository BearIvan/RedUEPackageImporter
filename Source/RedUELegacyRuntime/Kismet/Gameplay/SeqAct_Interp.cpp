#include "SeqAct_Interp.h"

#include "LevelSequencePlayer.h"

void USeqAct_Interp::BeginPlay()
{
	Super::BeginPlay();
}

void USeqAct_Interp::Play()
{
	if (ALevelSequenceActor* LevelSequenceActor = GetData())
	{
		if (ULevelSequencePlayer* LevelSequencePlayer = LevelSequenceActor->SequencePlayer)
		{
			LevelSequencePlayer->OnFinished.RemoveAll(this);
			LevelSequencePlayer->OnFinished.AddDynamic(this,&USeqAct_Interp::OnFinished);
			LevelSequencePlayer->Play();
			
		}
	}
}

void USeqAct_Interp::Reverse()
{
	if (ALevelSequenceActor* LevelSequenceActor = GetData())
	{
		if (ULevelSequencePlayer* LevelSequencePlayer = LevelSequenceActor->SequencePlayer)
		{
			LevelSequencePlayer->OnFinished.RemoveAll(this);
			LevelSequencePlayer->OnFinished.AddDynamic(this,&USeqAct_Interp::OnFinished);
			LevelSequencePlayer->OnNativeFinished.BindUObject(this,&USeqAct_Interp::OnFinished);
			LevelSequencePlayer->PlayReverse();
			Reversed.Broadcast();
		}
	}
}

void USeqAct_Interp::Stop()
{
	if (ALevelSequenceActor* LevelSequenceActor = GetData())
	{
		if (ULevelSequencePlayer* LevelSequencePlayer = LevelSequenceActor->SequencePlayer)
		{
			LevelSequencePlayer->OnFinished.RemoveAll(this);
			LevelSequencePlayer->Stop();
			Stopped.Broadcast();
		}
	}
}

void USeqAct_Interp::Pause()
{
	if (ALevelSequenceActor* LevelSequenceActor = GetData())
	{
		if (ULevelSequencePlayer* LevelSequencePlayer = LevelSequenceActor->SequencePlayer)
		{
			LevelSequencePlayer->OnFinished.RemoveAll(this);
			LevelSequencePlayer->Pause();
		}
	}
}

void USeqAct_Interp::Change()
{
	if (ALevelSequenceActor* LevelSequenceActor = GetData())
	{
		if (ULevelSequencePlayer* LevelSequencePlayer = LevelSequenceActor->SequencePlayer)
		{
			LevelSequencePlayer->OnFinished.RemoveAll(this);
			LevelSequencePlayer->OnFinished.AddDynamic(this,&USeqAct_Interp::OnFinished);
			if (LevelSequencePlayer->IsReversed())
			{
				LevelSequencePlayer->Play();
			}
			else
			{
				Reversed.Broadcast();
				LevelSequencePlayer->PlayReverse();
			}
		}
	}
}

void USeqAct_Interp::SetPosition()
{
	if (ALevelSequenceActor* LevelSequenceActor = GetData())
	{
		if (ULevelSequencePlayer* LevelSequencePlayer = LevelSequenceActor->SequencePlayer)
		{
			LevelSequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(GetNewPosition(),EUpdatePositionMethod::Jump));
			PositionSet.Broadcast();
		}
	}
}

void USeqAct_Interp::GetPositionAndDuration()
{
	if (ALevelSequenceActor* LevelSequenceActor = GetData())
	{
		if (ULevelSequencePlayer* LevelSequencePlayer = LevelSequenceActor->SequencePlayer)
		{
			SetCurrentPosition(LevelSequencePlayer->GetCurrentTime().AsSeconds());
			SetDuration(LevelSequencePlayer->GetDuration().AsSeconds());
			PositionAndDurationRetrieved.Broadcast();
		}
	}
}

void USeqAct_Interp::OnFinished()
{
	Completed.Broadcast();
}
