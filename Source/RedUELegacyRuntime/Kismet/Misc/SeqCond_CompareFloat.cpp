#include "Kismet/Misc/SeqCond_CompareFloat.h"

void USeqCond_CompareFloat::In()
{
	float A = GetValueA();
	float B = GetValueB();
	
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
