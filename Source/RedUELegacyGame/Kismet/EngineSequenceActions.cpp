// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineSequenceActions.h"

void USeqAct_SetMaterial::In()
{
}

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
