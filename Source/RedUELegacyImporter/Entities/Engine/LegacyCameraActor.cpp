#include "LegacyCameraActor.h"

UClass* ULegacyCameraActor::GetActorClass_Implementation()
{
	return ACameraActor::StaticClass();
}
