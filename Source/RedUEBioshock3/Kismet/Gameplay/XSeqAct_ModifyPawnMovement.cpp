#include "XSeqAct_ModifyPawnMovement.h"

void UXSeqAct_ModifyPawnMovement::Apply()
{
	Out.Broadcast();
}

void UXSeqAct_ModifyPawnMovement::Remove()
{
	Out.Broadcast();
}
