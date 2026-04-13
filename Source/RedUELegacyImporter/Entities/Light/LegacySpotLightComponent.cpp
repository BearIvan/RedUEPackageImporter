#include "Entities/Light/LegacySpotLightComponent.h"
#include "Components/SpotLightComponent.h"

void ULegacySpotLightComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	USpotLightComponent* SpotLightComponent =  CastChecked<USpotLightComponent>(InActorComponent);
	SpotLightComponent->InnerConeAngle = InnerConeAngle;
	SpotLightComponent->OuterConeAngle = OuterConeAngle;
}
