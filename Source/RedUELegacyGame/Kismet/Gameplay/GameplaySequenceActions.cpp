#include "GameplaySequenceActions.h"

void UXSeqAct_PlayScriptedSequence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentTime > 0.f)
	{
		CurrentTime -= DeltaTime;
		if (CurrentTime < 0.f)
		{
			StartPosition = GetStartPosition();
			if (StartPosition)
			{
				TArray<AActor*> InPlayers = GetTargets();
				for (AActor* InPlayer:InPlayers)
				{
					APawn*Pawn = Cast<APawn>(InPlayer);
					if (APlayerController* PlayerController = Cast<APlayerController>(InPlayer))
					{
						Pawn = PlayerController->GetPawn();
					}
					if (Pawn)
					{
						Pawn->TeleportTo(StartPosition->GetActorLocation(),StartPosition->GetActorRotation());
					}
				}
			}
		}
	}
}

void UXSeqAct_PlayScriptedSequence::Initiate()
{
	// if (CurrentTime > 0.f)
	// {
	// 	return;
	// }
	
	SequenceReady.Broadcast();
	if (CleanupAfterBeingReady)
	{
	 	SequenceCleanedUp.Broadcast();
	}
	 CurrentTime = MovePlayerDuration;

}

void UXSeqAct_PlayScriptedSequence::Cleanup()
{
	

	SequenceCleanedUp.Broadcast();
}

void UXSeqAct_HideHUDElement::HideElements()
{
	Out.Broadcast();
}

void UXSeqAct_HideHUDElement::ShowElements()
{
	Out.Broadcast();
}
void UXSeqAct_PatternMatcherSetGlobalFlag::Set()
{
	Out.Broadcast();
}

void UXSeqAct_PatternMatcherSetGlobalFlag::Unset()
{
	Out.Broadcast();
}

void UXSeqAct_PatternMatcherSetGlobalFlag::Toggle()
{
	Out.Broadcast();
}
void UXSeqAct_PlayerAbilityFilter::In()
{
	Out.Broadcast();
}

void UXSeqAct_ModifyPawnMovement::Apply()
{
	Out.Broadcast();
}

void UXSeqAct_ModifyPawnMovement::Remove()
{
	Out.Broadcast();
}
