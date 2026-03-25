#include "LegacyPrimitiveComponent.h"
#include "Entities/LegacyActor.h"
#include "Material/LegacyMaterialInterface.h"
#include "Particles/LegacyParticleSystem.h"

void ULegacyPrimitiveComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	USceneComponent* SceneComponent = CastChecked<USceneComponent>(InActorComponent);

	if (SceneComponent->GetAttachmentRootActor()->GetRootComponent() == InActorComponent)
	{
		return ;
	}
	SceneComponent->SetRelativeLocation(FVector(Translation));
	SceneComponent->SetRelativeRotation(FRotator(Rotation));
	SceneComponent->SetRelativeScale3D(FVector(Scale3D));
	SceneComponent->SetHiddenInGame(HiddenGame);
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(SceneComponent))
	{
		if (!CollideActors)
		{
			PrimitiveComponent->SetCollisionEnabled(BlockActors ? ECollisionEnabled::PhysicsOnly : ECollisionEnabled::NoCollision);
		}
		else if (!BlockActors)
		{
			PrimitiveComponent->SetCollisionEnabled(CollideActors ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		}
	}
}
