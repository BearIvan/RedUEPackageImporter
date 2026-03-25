
#include "LegacyLightComponent.h"

#include "Core/RedUELegacySubsystem.h"
#include "Material/LegacyMaterialInterface.h"

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
	
	LightComponent->SetIntensity(FMath::Min(Brightness,20.f));
	{
		URedUELegacySubsystem*RedUELegacySubsystem =  GetTypedOuter<URedUELegacySubsystem>();
		if (UCurveFloat* Curve = RedUELegacySubsystem->GetBrightnessToEVCurve())
		{
			LightComponent->SetIntensity(Curve->GetFloatValue(Brightness));
		}
	}
	if (Function)
	{
		if (Function->SourceMaterial)
		{
			if (UMaterialInterface* InSourceMaterial = Cast<UMaterialInterface>( Function->SourceMaterial->ExportToContent()))
			{
				LightComponent->SetLightFunctionMaterial(InSourceMaterial);
			}
		}
	}
	
	FQuat NeedRotation = FQuat(FRotator(90, 0, 0));
	FQuat SpotRotation = LightComponent->GetComponentQuat();
	SpotRotation = SpotRotation*NeedRotation;
	LightComponent->SetWorldRotation(SpotRotation);
}

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

void ULegacySpotLightComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	USpotLightComponent* SpotLightComponent =  CastChecked<USpotLightComponent>(InActorComponent);
	SpotLightComponent->InnerConeAngle = InnerConeAngle;
	SpotLightComponent->OuterConeAngle = OuterConeAngle;

	
	
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
