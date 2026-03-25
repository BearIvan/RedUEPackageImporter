#include "LegacyLight.h"
#include "LegacyLightComponent.h"
#include "Core/RedUELegacyArchive.h"

void ULegacyLight::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	ALight* LightActor = CastChecked<ALight>(InActor);
	if (LightComponent)
	{
		LightComponent->FillComponent(LightActor->GetLightComponent());
	}
}

UClass* ULegacySpotLight::GetActorClass_Implementation()
{
	return ASpotLight::StaticClass();
}

UClass* ULegacyPointLight::GetActorClass_Implementation()
{
	return APointLight::StaticClass();
}


void ULegacyStaticLightCollectionActor::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	Matrices.Empty();
	Matrices.AddZeroed(LightComponents.Num());
	for (int32 i = 0; i < LightComponents.Num(); i++)
	{
		Ar << Matrices[i];
	}
}

AActor* ULegacyStaticLightCollectionActor::Spawn_Implementation()
{
	for (int32 i = 0; i < LightComponents.Num(); i++)
	{
		FTransform3f Transform = FTransform3f(Matrices[i]);
		ULegacyLightComponent*Component = LightComponents[i];
		
		if (ULegacySpotLightComponent* SpotLight = Cast<ULegacySpotLightComponent>(Component))
		{
			ASpotLight* LightActor = GWorld->SpawnActor<ASpotLight>(FVector(Transform.GetLocation()),FRotator(Transform.GetRotation().Rotator()));
			LightActor->SetActorScale3D(FVector(Transform.GetScale3D()));
			Component->FillComponent(LightActor->SpotLightComponent);
		}
		else if (ULegacyPointLightComponent* PointLight = Cast<ULegacyPointLightComponent>(Component))
		{
			APointLight* LightActor = GWorld->SpawnActor<APointLight>(FVector(Transform.GetLocation()),FRotator(Transform.GetRotation().Rotator()));
			LightActor->SetActorScale3D(FVector(Transform.GetScale3D()));
			PointLight->FillComponent(LightActor->PointLightComponent);
		}
	}
	return nullptr;
}
