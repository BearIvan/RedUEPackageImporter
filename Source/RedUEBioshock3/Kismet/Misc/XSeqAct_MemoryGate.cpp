#include "Kismet/Misc/XSeqAct_MemoryGate.h"

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
