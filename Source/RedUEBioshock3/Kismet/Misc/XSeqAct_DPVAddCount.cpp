#include "XSeqAct_DPVAddCount.h"
#include "Blueprints/Bioshock3FunctionLibrary.h"
#include "Core/PlayerState/XPlayerStateComponent.h"

void UXSeqAct_DPVAddCount::In()
{
	if (UXPlayerStateComponent* PlayerState = UBioshock3FunctionLibrary::GetXPlayerStateComponent(GetPlayer()))
	{
		PlayerState->DPVAddCount(DesignerPlayerVariable, GetAmount());
	}
	Out.Broadcast();
}
