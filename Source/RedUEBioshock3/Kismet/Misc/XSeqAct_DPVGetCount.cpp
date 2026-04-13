#include "XSeqAct_DPVGetCount.h"

#include "Blueprints/Bioshock3FunctionLibrary.h"
#include "Core/PlayerState/XPlayerStateComponent.h"

void UXSeqAct_DPVGetCount::In()
{
	if (UXPlayerStateComponent* PlayerState = UBioshock3FunctionLibrary::GetXPlayerStateComponent(GetPlayer()))
	{
		SetCount(PlayerState->DPVGetCount(DesignerPlayerVariable));
	}
	Out.Broadcast();
}
