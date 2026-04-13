#include "LegacyVolume.h"

UClass* ULegacyTriggerVolume::GetActorClass_Implementation()
{
	return ATriggerVolume::StaticClass();
}
