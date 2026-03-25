#include "LegacyMarker.h"

UClass* ULegacyMarker::GetActorClass_Implementation()
{
	return ATargetPoint::StaticClass();
}
