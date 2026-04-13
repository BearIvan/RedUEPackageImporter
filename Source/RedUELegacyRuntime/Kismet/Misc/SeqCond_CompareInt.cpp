#include "Kismet/Misc/SeqCond_CompareInt.h"

void USeqCond_CompareInt::In()
{
	int32 A = GetValueA();
	int32 B = GetValueB();
	
	if (A <= B)
	{
		OutA.Broadcast();
	}
	
	if (A > B)
	{
		OutB.Broadcast();
	}
	
	if (A == B)
	{
		OutC.Broadcast();
	}
	
	if (A < B)
	{
		OutD.Broadcast();
	}
	
	if (A >= B)
	{
		OutE.Broadcast();
	}
}

void USeqCond_Increment::In()
{
	SetValueA(GetValueA() + IncrementAmount);
	Super::In();
}
