#include "XSeqAct_MultiLevelStreaming.h"
#include "Engine/LevelStreaming.h"

void UXSeqAct_MultiLevelStreaming::BeginPlay()
{
	Super::BeginPlay();
}

void UXSeqAct_MultiLevelStreaming::Construct()
{
	Super::Construct();
	for (FLegacyLevelStreamingNameCombo&Level:Levels)
	{
		Level.LevelStreaming = FStreamLevelAction::FindAndCacheLevelStreamingObject(Level.LevelName,GetWorld());
	}
}

void UXSeqAct_MultiLevelStreaming::Load()
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

void UXSeqAct_MultiLevelStreaming::Unload()
{
	for (FLegacyLevelStreamingNameCombo&Level:Levels)
	{
		if (ULevelStreaming* LevelStreaming = Level.LevelStreaming)
		{
			LevelStreaming->SetShouldBeLoaded(false);
		}
	}
}
