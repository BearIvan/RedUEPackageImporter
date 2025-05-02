#include "World/Components/LegacyPrimitiveComponent.h"

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
