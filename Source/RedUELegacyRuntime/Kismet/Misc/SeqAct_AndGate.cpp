#include "Kismet/Misc/SeqAct_AndGate.h"

void USeqAct_AndGate::In()
{
	if (bOpen)
	{
		Out.Broadcast();
		bOpen = false;
	}
}

void USeqAct_AndGate::Reset()
{
	bOpen = true;
}
