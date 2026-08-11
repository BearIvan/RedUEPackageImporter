#include "XSeqAct_SmartTerrainMultiPhaseCustomAnim_Request.h"

#include "Interfaces/RedXPawnInterface.h"

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
