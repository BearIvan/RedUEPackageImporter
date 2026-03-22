#include "SeqAct_JoinFloatingSection.h"
#include "EngineUtils.h"
#include "Actors/XFloatingSection.h"

void USeqAct_JoinFloatingSection::In()
{
	for (TActorIterator<AXFloatingSection> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->LevelNames.Contains(FloatingSectionActor))
		{
			TArray<AActor*> InTargets = GetTargets();
			for (AActor* Actor:InTargets)
			{
				if (AController* Controller = Cast<AController>(Actor))
				{
					Actor = Controller->GetPawn();
				}
				if (Actor)
				{
					Actor->AttachToActor(*ActorItr,FAttachmentTransformRules::KeepRelativeTransform);
				}
			}
			break;
		}
	}
	Out.Broadcast();
}
