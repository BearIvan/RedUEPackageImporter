#include "Kismet/Engine/SeqAct_Log.h"

void USeqAct_Log::In()
{
	Out.Broadcast();
}
