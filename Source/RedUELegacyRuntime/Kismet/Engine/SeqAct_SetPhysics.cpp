#include "Kismet/Engine/SeqAct_SetPhysics.h"

void USeqAct_SetPhysics::In()
{
	Out.Broadcast();
}
