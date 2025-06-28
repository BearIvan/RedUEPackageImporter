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
	SetOutBoolean(GetValue());
	Out.Broadcast();
}
