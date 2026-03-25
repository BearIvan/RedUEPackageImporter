#include "LegacyWorldInfo.h"

void ULegacyWorldInfo::GetStreamingLevels(TArray<ULegacyLevelStreaming*>& OutStreamingLevels)
{
	OutStreamingLevels = StreamingLevels;
}

UClass* ULegacyWorldInfo::GetActorClass_Implementation()
{
	return nullptr;
}
