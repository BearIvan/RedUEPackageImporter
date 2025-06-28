#include "Actors/MaterialInstanceActor/MaterialInstanceHybridActor.h"


AMaterialInstanceHybridActor::AMaterialInstanceHybridActor(): MaterialInstance(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}
