#include "World/Actors/LegacySkeletalMeshActor.h"
#include "Animation/SkeletalMeshActor.h"
#include "Material/LegacyMaterialInterface.h"
#include "World/Components/LegacySkeletalMeshComponent.h"

void ULegacySkeletalMeshActor::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	ASkeletalMeshActor* MeshActor = CastChecked<ASkeletalMeshActor>(InActor);
	SkeletalMeshComponent->FillComponent(MeshActor->GetSkeletalMeshComponent());
}

UClass* ULegacySkeletalMeshActor::GetActorClass_Implementation()
{
	return ASkeletalMeshActor::StaticClass();
}
