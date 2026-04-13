#include "LegacyActorComponent.h"


void ULegacyActorComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
    InActorComponent->bAutoActivate = bAutoActivate;
}
