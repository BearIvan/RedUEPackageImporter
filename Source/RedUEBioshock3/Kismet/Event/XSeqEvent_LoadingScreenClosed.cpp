#include "Kismet/Event/XSeqEvent_LoadingScreenClosed.h"

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
