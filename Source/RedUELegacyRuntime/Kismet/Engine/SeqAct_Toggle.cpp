#include "Kismet/Engine/SeqAct_Toggle.h"

void USeqAct_Toggle::TurnOn()
{
	Out.Broadcast();
}

void USeqAct_Toggle::TurnOff()
{
	Out.Broadcast();
}

void USeqAct_Toggle::Toggle()
{
	Out.Broadcast();
}
