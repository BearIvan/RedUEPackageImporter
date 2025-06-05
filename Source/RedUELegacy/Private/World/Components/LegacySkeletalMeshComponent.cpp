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
				if (AnimSet)
				{
					AnimSet->ImportSequences(InSkeletalMesh->GetSkeleton());
				}
			}
		}
	
		if (ULegacySkeletalMesh3*SkeletalMesh3 = Cast<ULegacySkeletalMesh3>(SkeletalMesh))
		{
			AActor* OuterActor = InActorComponent->GetOwner();
			if (SkeletalMeshComponent == OuterActor->GetRootComponent())
			{
				if ((FQuat4f::Identity.Equals(FQuat4f(SkeletalMesh3->RotOrigin))&&SkeletalMesh3->MeshOrigin.IsNearlyZero()) )
				{
					return;
				}
			}
			
			FMatrix Transform = SkeletalMeshComponent->GetRelativeTransform().ToMatrixWithScale();
			FMatrix RelativeTransform = FTransform(SkeletalMesh3->RotOrigin,FVector(SkeletalMesh3->MeshOrigin)).ToMatrixWithScale();
			Transform = RelativeTransform*Transform;
			SkeletalMeshComponent->SetRelativeTransform(FTransform(Transform));
		}
	}
	
}
