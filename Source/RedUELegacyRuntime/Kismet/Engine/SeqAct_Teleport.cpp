#include "Kismet/Engine/SeqAct_Teleport.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void USeqAct_Teleport::In()
{
	AActor* InDestination = GetDestination();
	if (InDestination)
	{
		TArray<AActor*> InTargets = GetTargets();
		for (AActor* Actor:InTargets)
		{
			if (APlayerController*PC = Cast<APlayerController>(Actor))
			{
				PC->GetPawn()->TeleportTo(InDestination->GetActorLocation(),InDestination->GetActorRotation());
			}
			else if (Actor)
			{
				Actor->TeleportTo(InDestination->GetActorLocation(),InDestination->GetActorRotation());
			}
		}
	}
	Out.Broadcast();
}
