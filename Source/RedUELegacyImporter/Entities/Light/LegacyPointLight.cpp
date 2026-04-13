#include "Entities/Light/LegacyPointLight.h"
#include "Components/PointLightComponent.h"

UClass* ULegacyPointLight::GetActorClass_Implementation()
{
	return APointLight::StaticClass();
}
