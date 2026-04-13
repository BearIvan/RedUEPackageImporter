#include "SeqAct_MultiLevelStreaming.h"


void USeqAct_MultiLevelStreaming::BeginPlay()
{
	Super::BeginPlay();
}

void USeqAct_MultiLevelStreaming::Construct()
{
	Super::Construct();
	for (FLegacyLevelStreamingNameCombo&Level:Levels)
	{
		Level.LevelStreaming = FStreamLevelAction::FindAndCacheLevelStreamingObject(Level.LevelName,GetWorld());
	}
}

void USeqAct_MultiLevelStreaming::Load()
{
	for (FLegacyLevelStreamingNameCombo&Level:Levels)
	{
		if (ULevelStreaming* LevelStreaming = Level.LevelStreaming)
		{
			LevelStreaming->SetShouldBeLoaded(true);
			LevelStreaming->SetShouldBeVisible(true);
		}
	}
}

void USeqAct_MultiLevelStreaming::Unload()
{
	for (FLegacyLevelStreamingNameCombo&Level:Levels)
	{
		if (ULevelStreaming* LevelStreaming = Level.LevelStreaming)
		{
			LevelStreaming->SetShouldBeLoaded(false);
		}
	}
}
