#include "World/Actors/LegacyStaticMeshActor.h"
#include "World/Components/LegacyStaticMeshComponent.h"

void ULegacyStaticMeshActor::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	AStaticMeshActor* StaticMeshActor = CastChecked<AStaticMeshActor>(InActor);
    StaticMeshComponent->FillComponent(StaticMeshActor->GetStaticMeshComponent());
}

UClass* ULegacyStaticMeshActor::GetActorClass_Implementation()
{
	return AStaticMeshActor::StaticClass();
}
