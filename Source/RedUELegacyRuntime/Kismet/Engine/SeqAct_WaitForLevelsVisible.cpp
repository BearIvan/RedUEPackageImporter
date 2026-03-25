#include "Kismet/Engine/SeqAct_WaitForLevelsVisible.h"
#include "Engine/LevelStreaming.h"

void USeqAct_WaitForLevelsVisible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!Levels.IsEmpty())
	{
		for (ULevelStreaming* LevelStreaming : Levels)
		{
			if (LevelStreaming->IsLevelVisible())
			{
				Levels.Remove(LevelStreaming);
			}
		}
	
		if (Levels.IsEmpty())
		{
			Finished.Broadcast();
		}
	}
}

void USeqAct_WaitForLevelsVisible::Wait()
{
	Levels.Empty();
	
	class UWorld* InWorld = GetWorld();
	for (FName LevelName:LevelNames)
	{
		if (ULevelStreaming* LocalLevel = FStreamLevelAction::FindAndCacheLevelStreamingObject( LevelName, InWorld ))
		{
			Levels.Add(LocalLevel);
			LocalLevel->bShouldBlockOnLoad = bShouldBlockOnLoad;
		}
	}
	if (Levels.IsEmpty())
	{
		Finished.Broadcast();
	}
}
