#include "World/Components/LegacyActorComponent.h"


void ULegacyActorComponent::LegacySerializeComponent(FArchive& Ar)
{
    Super::LegacySerializeComponent(Ar);
}

void ULegacyActorComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
    InActorComponent->bAutoActivate = bAutoActivate;
}
