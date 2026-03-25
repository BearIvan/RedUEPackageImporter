#include "XSeqAct_UseSmartTerrain.h"

#include "Interfaces/RedXPawnInterface.h"


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
