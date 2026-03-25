#include "Kismet/Engine/SeqAct_Delay.h"

USeqAct_Delay::USeqAct_Delay()
{
	CurrentTime = 0;
	Paused = false;
}

void USeqAct_Delay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentTime > 0 && !Paused)
	{
		CurrentTime -= DeltaTime;
		if (CurrentTime <= 0)
		{
			Finished.Broadcast();
		}
	}
}

void USeqAct_Delay::Start()
{
	if (Paused)
	{
		Paused = false;
		return;
	}
	CurrentTime = GetDuration();
}

void USeqAct_Delay::Stop()
{
	if (CurrentTime > 0)
	{
		Aborted.Broadcast();
	}
	Paused = false;
	CurrentTime = 0;
}

void USeqAct_Delay::Pause()
{
	if (CurrentTime > 0)
	{
		Paused = true;
	}
}
