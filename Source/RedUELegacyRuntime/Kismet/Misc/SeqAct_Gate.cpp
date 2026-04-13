#include "Kismet/Misc/SeqAct_Gate.h"

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
			Closed.Broadcast();
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
	Closed.Broadcast();
}

void USeqAct_Gate::Toggle()
{
	bOpen = !bOpen;
	if (!bOpen)
	{
		Closed.Broadcast();
	}
	CurrentCloseCount = AutoCloseCount;
}
