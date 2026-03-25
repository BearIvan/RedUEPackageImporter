#include "Kismet/Misc/SeqAct_SetInt.h"

void USeqAct_SetInt::In()
{
	int32 OutTarget = 0;
	
	TArray<int32> InValues = GetValue();
	for (int32 InValue: InValues)
	{
		OutTarget += InValue;
	}
	
	SetTarget(OutTarget);
	Out.Broadcast();
}
