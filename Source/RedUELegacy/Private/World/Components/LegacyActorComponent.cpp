// Tyran


#include "World/Components/LegacyActorComponent.h"

void ULegacyActorComponent::LegacySerializeComponent(FArchive& Ar)
{
    Super::LegacySerializeComponent(Ar);
    UObject* TemplateOwnerClass;
    Ar<<TemplateOwnerClass;
}

void ULegacyActorComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
}
