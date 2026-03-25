#include "Kismet/Misc/SeqCond_CompareBool.h"

void USeqCond_CompareBool::In()
{
	if (GetInBoolean())
	{
		OutTrue.Broadcast();
	}
	else
	{
		OutFalse.Broadcast();
	}
}
