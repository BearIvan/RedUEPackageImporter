#include "World/Actors/LegacyLight.h"
#include "World/Components/LegacyLightComponent.h"

void ULegacyLight::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	ALight* LightActor = CastChecked<ALight>(InActor);
	LightComponent->FillComponent(LightActor->GetLightComponent());
}

UClass* ULegacySpotLight::GetActorClass_Implementation()
{
	return ASpotLight::StaticClass();
}

UClass* ULegacyPointLight::GetActorClass_Implementation()
{
	return APointLight::StaticClass();
}
