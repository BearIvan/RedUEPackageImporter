#include "LegacyXWorldInfo.h"

void ULegacyXWorldInfo::GetStreamingLevels(TArray<ULegacyLevelStreaming*>& OutStreamingLevels)
{
	OutStreamingLevels = StreamingLevelsWrapper.StreamingLevels;
}