#include "SeqEvent_LevelLoaded.h"

void USeqEvent_LevelLoaded::BeginPlay()
{
	Super::BeginPlay();	
	MatchHasBegun.Broadcast();
	LoadedAndVisible.Broadcast();
}

void UXSeqEvent_LoadingScreenClosed::BeginPlay()
{
	Super::BeginPlay();
	switch (RequiredLoadingScreenType)
	{
	case ELegacyRequiredLoadingScreenType::ERLST_Any:
	case ELegacyRequiredLoadingScreenType::ERLST_NonCheckpoint:
		Out.Broadcast();
		break;
	case ELegacyRequiredLoadingScreenType::ERLST_Checkpoint:
		break;
	case ELegacyRequiredLoadingScreenType::ERLST_MAX:
		break;
	default: ;
	}
}
