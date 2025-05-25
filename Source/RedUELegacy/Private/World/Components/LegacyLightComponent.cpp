#include "World/Components/LegacyLightComponent.h"

void ULegacyLightComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	ULightComponent* LightComponent =  CastChecked<ULightComponent>(InActorComponent);
	LightComponent->LightColor = LightColor;
	LightComponent->Intensity = Brightness;
}

void ULegacyPointLightComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	
	UPointLightComponent* PointLightComponent =  CastChecked<UPointLightComponent>(InActorComponent);
	if (PointLightComponent->GetAttachmentRootActor()->GetRootComponent() != InActorComponent)
	{
		PointLightComponent->SetRelativeLocation(FVector(Translation));
	}
	PointLightComponent->SetSourceRadius(Radius);
}

void ULegacySpotLightComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	USpotLightComponent* SpotLightComponent =  CastChecked<USpotLightComponent>(InActorComponent);
	SpotLightComponent->InnerConeAngle = InnerConeAngle;
	SpotLightComponent->OuterConeAngle = OuterConeAngle;
	if (SpotLightComponent->GetAttachmentRootActor()->GetRootComponent() != InActorComponent)
	{
		SpotLightComponent->SetRelativeRotation(Rotation);
	}
	
}
