#include "XSeqAct_PlayScriptedSequence.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

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
