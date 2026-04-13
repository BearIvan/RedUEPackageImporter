#include "LegacyNavigationPoint.h"

UClass* ULegacyNavigationPoint::GetActorClass_Implementation()
{
	return ANavigationObjectBase::StaticClass();
}
