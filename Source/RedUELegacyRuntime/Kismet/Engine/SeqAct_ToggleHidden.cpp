#include "Kismet/Engine/SeqAct_ToggleHidden.h"

void USeqAct_ToggleHidden::Hide()
{
	TArray<AActor*> InActors = GetActors();
	for (AActor* Actor : InActors)
	{
		if (!Actor)
		{
			continue;
		}
		Actor->SetActorHiddenInGame(true);
	}
	Out.Broadcast();
}

void USeqAct_ToggleHidden::UnHide()
{
	TArray<AActor*> InActors = GetActors();
	for (AActor* Actor : InActors)
	{
		if (!Actor)
		{
			continue;
		}
		Actor->SetActorHiddenInGame(false);
	}
	Out.Broadcast();
}

void USeqAct_ToggleHidden::Toggle()
{
	TArray<AActor*> InActors = GetActors();
	for (AActor* Actor : InActors)
	{
		if (!Actor)
		{
			continue;
		}
		Actor->SetActorHiddenInGame(!Actor->IsHidden());
	}
	Out.Broadcast();
}
