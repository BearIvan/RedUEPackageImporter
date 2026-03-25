#include "Kismet/Misc/SeqAct_SetBool.h"

void USeqAct_SetBool::In()
{
	bool bResult  = true;
	TArray<bool> InValues = GetValues();
	if (InValues.IsEmpty())
	{
		InValues.Add(Value);
	}
	for (bool InValue: InValues)
	{
		bResult &= InValue;
	}
	
	SetTarget(bResult);
	Out.Broadcast();
}
