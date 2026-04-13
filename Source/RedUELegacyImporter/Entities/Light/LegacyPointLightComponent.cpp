#include "Entities/Light/LegacyPointLightComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"

void ULegacyPointLightComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	
	UPointLightComponent* PointLightComponent =  CastChecked<UPointLightComponent>(InActorComponent);
	if (PointLightComponent->GetAttachmentRootActor()->GetRootComponent() != InActorComponent)
	{
		PointLightComponent->SetRelativeLocation(FVector(Translation));
	}
	PointLightComponent->SetAttenuationRadius(Radius*2.f);
	PointLightComponent->SetUseInverseSquaredFalloff(false);
	PointLightComponent->SetLightFalloffExponent(FalloffExponent);
}
