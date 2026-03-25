#include "Kismet/Engine/SeqAct_SetObject.h"

void USeqAct_SetObject::In()
{
	SetTargets(GetValue());
	Out.Broadcast();
}
