#include "Kismet/Misc/SeqAct_SetString.h"

void USeqAct_SetString::In()
{
	SetTarget(GetValue());
	Out.Broadcast();
}
