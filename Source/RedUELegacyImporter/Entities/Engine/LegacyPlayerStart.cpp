#include "LegacyPlayerStart.h"

UClass* ULegacyPlayerStart::GetActorClass_Implementation()
{
	return APlayerStart::StaticClass();
}