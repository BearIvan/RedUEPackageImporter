#include "LegacySkeletalMeshActor.h"
#include "LegacySkeletalMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"

void ULegacySkeletalMeshActor::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	ASkeletalMeshActor* MeshActor = CastChecked<ASkeletalMeshActor>(InActor);
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->FillComponent(MeshActor->GetSkeletalMeshComponent());
	}
}

UClass* ULegacySkeletalMeshActor::GetActorClass_Implementation()
{
	return ASkeletalMeshActor::StaticClass();
}
