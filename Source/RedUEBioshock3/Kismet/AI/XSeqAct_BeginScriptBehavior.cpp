#include "XSeqAct_BeginScriptBehavior.h"

void UXSeqAct_BeginScriptBehavior::In()
{
	Out.Broadcast();
	Done.Broadcast();
}
