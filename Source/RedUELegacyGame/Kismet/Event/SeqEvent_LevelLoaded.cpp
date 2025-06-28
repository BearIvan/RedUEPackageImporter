#include "SeqEvent_LevelLoaded.h"

void USeqEvent_LevelLoaded::BeginPlay()
{
	Super::BeginPlay();
	MatchHasBegun.Broadcast();
	LoadedAndVisible.Broadcast();
}
