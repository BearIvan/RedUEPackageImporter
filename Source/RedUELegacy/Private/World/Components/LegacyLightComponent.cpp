#include "World/Components/LegacyLightComponent.h"

void ULegacyLightComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	ULightComponent* LightComponent =  CastChecked<ULightComponent>(InActorComponent);
	LightComponent->LightColor = LightColor;
	if (ULocalLightComponent* LocalLightComponent = CastChecked<ULocalLightComponent>(LightComponent))
	{
		LocalLightComponent->IntensityUnits = ELightUnits::EV;
	}
	LightComponent->SetMobility( EComponentMobility::Movable);
	LightComponent->SetIntensity(Brightness);
	
}

void ULegacyPointLightComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	
	UPointLightComponent* PointLightComponent =  CastChecked<UPointLightComponent>(InActorComponent);
	if (PointLightComponent->GetAttachmentRootActor()->GetRootComponent() != InActorComponent)
	{
		PointLightComponent->SetRelativeLocation(FVector(Translation));
	}
	PointLightComponent->SetAttenuationRadius(Radius);
}

void ULegacySpotLightComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	USpotLightComponent* SpotLightComponent =  CastChecked<USpotLightComponent>(InActorComponent);
	SpotLightComponent->InnerConeAngle = InnerConeAngle;
	SpotLightComponent->OuterConeAngle = OuterConeAngle;
	static FQuat NeedRotation = FQuat(FRotator(-90.f, 0, 0));
	FQuat SpotRotation = SpotLightComponent->GetComponentQuat();
	SpotRotation = SpotRotation*NeedRotation;
	SpotLightComponent->SetWorldRotation(SpotRotation);
	
	
	// if (SpotLightComponent->GetAttachmentRootActor()->GetRootComponent() != InActorComponent)
	// {
	// 	Rotation.Pitch -= 90.f;
	// 	SpotLightComponent->SetRelativeRotation(Rotation);
	// }
	// else
	// {
	// 	FRotator Rotator = SpotLightComponent->GetComponentRotation();
	// 	Rotator.Pitch -= 90.f;
	// 	SpotLightComponent->SetWorldRotation(Rotator);
	// }
	
}
