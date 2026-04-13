#include "LegacyLight.h"
#include "Entities/Light/LegacyLightComponent.h"

void ULegacyLight::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	ALight* LightActor = CastChecked<ALight>(InActor);
	if (LightComponent)
	{
		LightComponent->FillComponent(LightActor->GetLightComponent());
	}
}
