#include "Kismet/Misc/SeqAct_SetFloat.h"

void USeqAct_SetFloat::In()
{
	float OutTarget = 0;
	
	TArray<float> InValues = GetValue();
	for (float InValue: InValues)
	{
		OutTarget += InValue;
	}
	
	SetTarget(OutTarget);
	Out.Broadcast();
}
