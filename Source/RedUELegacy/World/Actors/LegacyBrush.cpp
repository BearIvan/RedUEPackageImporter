#include "World/Actors/LegacyBrush.h"
#include "World/Components/LegacyBrushComponent.h"

void ULegacyBrush::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	
	if (ABrush* BrushActor = Cast<ABrush>(InActor))
	{
		BrushComponent->FillComponent(BrushActor->GetBrushComponent());
	}
	
}
