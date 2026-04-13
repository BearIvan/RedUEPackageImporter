#include "Entities/Light/LegacyStaticLightCollectionActor.h"
#include "Entities/Light/LegacyLightComponent.h"
#include "Entities/Light/LegacyPointLightComponent.h"
#include "Entities/Light/LegacySpotLightComponent.h"
#include "Core/RedUELegacyArchive.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"

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
			LightActor->SetFolderPath(*GetLegacyName());
			LightActor->SetActorScale3D(FVector(Transform.GetScale3D()));
			SpotLight->FillComponent(LightActor->SpotLightComponent);
		}
		else if (ULegacyPointLightComponent* PointLight = Cast<ULegacyPointLightComponent>(Component))
		{
			APointLight* LightActor = GWorld->SpawnActor<APointLight>(FVector(Transform.GetLocation()),FRotator(Transform.GetRotation().Rotator()));
			LightActor->SetFolderPath(*GetLegacyName());
			LightActor->SetActorScale3D(FVector(Transform.GetScale3D()));
			PointLight->FillComponent(LightActor->PointLightComponent);
		}
	}
	return nullptr;
}
