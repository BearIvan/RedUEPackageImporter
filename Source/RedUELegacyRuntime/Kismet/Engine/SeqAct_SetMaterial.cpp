#include "Kismet/Engine/SeqAct_SetMaterial.h"
#include "Components/PrimitiveComponent.h"

void USeqAct_SetMaterial::In()
{
	TArray<AActor*> InActors = GetActors();
	for (AActor* Actor : InActors)
	{
		if (Actor)
		{
			TArray<UPrimitiveComponent*> PrimitiveComponents;
			Actor->GetComponents(PrimitiveComponents);
			for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
			{
				PrimitiveComponent->SetMaterial(MaterialIndex,NewMaterial);
			}
		}
	}
	Out.Broadcast();
}
