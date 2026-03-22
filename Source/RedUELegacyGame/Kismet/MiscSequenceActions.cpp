#include "MiscSequenceActions.h"

void USeqCond_CompareBool::In()
{
	if (GetInBoolean())
	{
		OutTrue.Broadcast();
	}
	else
	{
		OutFalse.Broadcast();
	}
}

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

void USeqAct_AndGate::In()
{
	if (bOpen)
	{
		Out.Broadcast();
		bOpen = false;
	}
}

void USeqAct_AndGate::Reset()
{
	bOpen = true;
}

USeqAct_Gate::USeqAct_Gate()
{
}

void USeqAct_Gate::BeginPlay()
{
	Super::BeginPlay();
	CurrentCloseCount = AutoCloseCount;
}

void USeqAct_Gate::In()
{
	bool bWasOpened = bOpen;
	if (AutoCloseCount)
	{
		if (--CurrentCloseCount == 0)
		{
			bOpen = false;
		}
	}
	
	if (bWasOpened)
	{
		Out.Broadcast();
	}
}

void USeqAct_Gate::Open()
{
	bOpen = true;
	CurrentCloseCount = AutoCloseCount;
	
}

void USeqAct_Gate::Close()
{
	bOpen = false;
}

void USeqAct_Gate::Toggle()
{
	bOpen = !bOpen;
	CurrentCloseCount = AutoCloseCount;
}

UXSeqAct_MemoryGate::UXSeqAct_MemoryGate()
{
}

void UXSeqAct_MemoryGate::BeginPlay()
{
	Super::BeginPlay();
}

void UXSeqAct_MemoryGate::In()
{
	bool bWasOpened = bOpen;
	if (AutoCloseCount)
	{
		if (--CurrentCloseCount == 0)
		{
			bOpen = false;
		}
	}
	
	if (bWasOpened)
	{
		Out.Broadcast();
	}
}

void UXSeqAct_MemoryGate::Open()
{
	bOpen = true;
	if (AutoCloseCount&&CurrentCloseCount  == 0)
	{
		bOpen = false;
	}
}

void UXSeqAct_MemoryGate::Close()
{
	bOpen = false;
}

void UXSeqAct_MemoryGate::Toggle()
{
	bOpen = !bOpen;
	if (AutoCloseCount&&CurrentCloseCount  == 0)
	{
		bOpen = false;
	}
}

void UXSeqAct_MemoryGate::Clear()
{
	CurrentCloseCount = AutoCloseCount;
	bOpen = true;
}


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

void USeqAct_SetString::In()
{
	SetTarget(GetValue());
	Out.Broadcast();
}

void USeqAct_Switch::In()
{
	for (INT Idx = 0; Idx < Indices.Num(); Idx++)
	{
		INT ActiveIdx = Indices[Idx] - 1;
		if (ActiveIdx >= 0 && ActiveIdx < CustomLinks.Num())
		{
			if (!DisabledIndices.Contains(ActiveIdx))
			{
				ExecuteCustomLink(ActiveIdx);
				if (bAutoDisableLinks)
				{
					DisabledIndices.Add(ActiveIdx);
				}
			}
		}
		// increment the indices
		if (IncrementAmount != 0)
		{
			if (bLooping)
			{
				Indices[Idx] = 1 + ((Indices[Idx] - 1 + IncrementAmount) %  CustomLinks.Num());
			}
			else
			{
				Indices[Idx] += IncrementAmount;
			}
		}
	}
}
