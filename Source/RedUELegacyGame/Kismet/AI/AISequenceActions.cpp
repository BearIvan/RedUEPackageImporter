#include "AISequenceActions.h"

#include "Interfaces/Bioshock/Pawn/RedXPawnInterface.h"

void UXSeqAct_BeginScriptBehavior::In()
{
	Out.Broadcast();
	Done.Broadcast();
}

void UXSeqAct_EndScriptBehavior::In()
{
}

void UXSeqAct_SmartTerrainMultiPhaseCustomAnim_Request::SendRequests()
{
	TArray<AActor*>Actors = GetTargets();
	for (AActor* Actor : Actors)
	{
		if (APawn* Pawn = Cast<APawn>(Actor))
		{
			IRedXPawnInterface::Execute_SendCurrentSmartTerrain(Pawn,RequestsToSend);
		}
	}
	Out.Broadcast();
}

void UXSeqAct_UseSmartTerrain::In()
{
	TArray<AActor*>Actors = GetTargets();
	for (AActor* Actor : Actors)
	{
		if (APawn* Pawn = Cast<APawn>(Actor))
		{
			IRedXPawnInterface::Execute_SetSmartTerrain(Pawn,SmartTerrainName);
		}
	}
	Out.Broadcast();
}


void UXSeqAct_StopSmartTerrain::In()
{
}

void UXSeqAct_StopSmartTerrain::RequestOutro()
{
}

void UXSeqAct_TeleportElizabethToPlayer::In()
{
	Out.Broadcast();
}
