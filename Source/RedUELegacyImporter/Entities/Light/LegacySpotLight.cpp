#include "Entities/Light/LegacySpotLight.h"
#include "Components/SpotLightComponent.h"

UClass* ULegacySpotLight::GetActorClass_Implementation()
{
	return ASpotLight::StaticClass();
}
