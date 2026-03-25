#include "World/Components/LegacyStaticMeshComponent.h"

ULegacyStaticMeshComponent::ULegacyStaticMeshComponent()
{
	CollideActors = true;
	BlockActors = true;
}

void ULegacyStaticMeshComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);

	UStaticMeshComponent* InStaticMeshComponent = CastChecked<UStaticMeshComponent>(InActorComponent);
	if(StaticMesh)
	{
		if(UStaticMesh* InStaticMesh = CastChecked<UStaticMesh>(StaticMesh->ExportToContent(),ECastCheckedType::NullAllowed))
		{
			InStaticMeshComponent->SetStaticMesh(InStaticMesh);
			for(int32  i = 0; i < Materials.Num();i++)
			{
				if(Materials[i])
				{
					InStaticMeshComponent->SetMaterial(i,CastChecked<UMaterialInterface>(Materials[i]->ExportToContent(),ECastCheckedType::NullAllowed));
				}
			}
		}
	}
	InStaticMeshComponent->SetCastHiddenShadow(bCastHiddenShadow);
	
}
