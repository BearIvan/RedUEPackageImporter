#include "World/Components/LegacyPrimitiveComponent.h"

#include "Material/LegacyMaterialInterface.h"
#include "Materials/MaterialInstanceActor.h"
#include "Particles/LegacyParticleSystem.h"
#include "World/Actors/LegacyActor.h"

void ULegacyPrimitiveComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	USceneComponent* SceneComponent = CastChecked<USceneComponent>(InActorComponent);

	if (SceneComponent->GetAttachmentRootActor()->GetRootComponent() == InActorComponent)
	{
		return;
	}
	SceneComponent->SetRelativeLocation(FVector(Translation));
	SceneComponent->SetRelativeRotation(FRotator(Rotation));
	SceneComponent->SetRelativeScale3D(FVector(Scale3D));
}

void ULegacyParticleSystemComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	UParticleSystemComponent* ParticleSystemComponent = CastChecked<UParticleSystemComponent>(InActorComponent);
	ParticleSystemComponent->SetTemplate( Cast<UParticleSystem>(Template->ExportToContent()) );
	ParticleSystemComponent->PartSysVelocity = FVector(PartSysVelocity);
	ParticleSystemComponent->bSuppressSpawning = bSuppressSpawning;
	ParticleSystemComponent->bWasDeactivated = bWasDeactivated;
	ParticleSystemComponent->InstanceParameters.Empty();
	for (const FLegacyParticleSysParam& InstanceParameter : InstanceParameters)
	{
		FParticleSysParam& NewParameter = ParticleSystemComponent->InstanceParameters.AddDefaulted_GetRef();
		if (InstanceParameter.Actor)
		{
			NewParameter.Actor = Cast<AActor>(InstanceParameter.Actor->PresentObject);
		}
		if (NewParameter.Material)
		{
			NewParameter.Material = Cast<UMaterialInterface>(InstanceParameter.Material->ExportToContent());
		}
		NewParameter.Color = InstanceParameter.Color;
		NewParameter.Name = InstanceParameter.Name;
		NewParameter.ParamType = InstanceParameter.ParamType;
		NewParameter.Scalar = InstanceParameter.Scalar;
		NewParameter.Scalar_Low = InstanceParameter.Scalar;
		NewParameter.Vector = FVector(InstanceParameter.Vector);
		NewParameter.Vector_Low = FVector(InstanceParameter.Vector);
	}
}
