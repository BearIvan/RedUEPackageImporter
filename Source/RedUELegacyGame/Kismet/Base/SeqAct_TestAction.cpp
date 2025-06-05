#include "SeqAct_TestAction.h"

void USeqAct_TestAction::BeginPlay()
{
	Super::BeginPlay();
}

void USeqAct_TestAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timer-=DeltaTime;
	if (Timer <= 0)
	{
		Timer = TimerInterval;
		SetTest123(!GetTest123());
		OnFinish.Broadcast();
	}
}

void USeqAct_TestAction::Test1()
{
	UE_DEBUG_BREAK();
}

void USeqAct_TestAction::Test2()
{
	OnTest2.Broadcast();
}
