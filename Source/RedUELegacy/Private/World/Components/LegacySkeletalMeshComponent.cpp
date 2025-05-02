#include "World/Components/LegacySkeletalMeshComponent.h"
#include "Material/LegacyMaterialInterface.h"
#include "Mesh/LegacyAnimSet.h"
#include "Mesh/LegacySkeletalMesh3.h"

void ULegacySkeletalMeshComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	USkeletalMeshComponent* SkeletalMeshComponent = CastChecked<USkeletalMeshComponent>(InActorComponent);
	if(SkeletalMesh)
	{
		if(USkeletalMesh* InSkeletalMesh = CastChecked<USkeletalMesh>(SkeletalMesh->ExportToContent(),ECastCheckedType::NullAllowed))
		{
			SkeletalMeshComponent->SetSkeletalMesh(InSkeletalMesh);
			for(int32  i = 0; i < Materials.Num();i++)
			{
				if(Materials[i])
				{
					SkeletalMeshComponent->SetMaterial(i,CastChecked<UMaterialInterface>(Materials[i]->ExportToContent(),ECastCheckedType::NullAllowed));
				}
			}
			for (ULegacyAnimSet*AnimSet:AnimSets)
			{
				AnimSet->ImportSequences(InSkeletalMesh->GetSkeleton());
			}
		}
		if (ULegacySkeletalMesh3*SkeletalMesh3 = Cast<ULegacySkeletalMesh3>(SkeletalMesh))
		{
			FMatrix Transform = SkeletalMeshComponent->GetRelativeTransform().ToMatrixWithScale();
			FMatrix RelativeTransform = FTransform(SkeletalMesh3->RotOrigin,FVector(SkeletalMesh3->MeshOrigin)).ToMatrixWithScale();
			Transform = RelativeTransform*Transform;
			SkeletalMeshComponent->SetRelativeTransform(FTransform(Transform));
		}
	}
	
}
