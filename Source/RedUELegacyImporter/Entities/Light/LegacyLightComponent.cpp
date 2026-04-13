#include "Entities/Light/LegacyLightComponent.h"
#include "Core/RedUELegacySubsystem.h"
#include "Material/LegacyMaterialInterface.h"
#include "Components/LightComponent.h"
#include "Components/LocalLightComponent.h"
#include "Curves/CurveFloat.h"

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
