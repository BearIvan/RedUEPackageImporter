#include "LegacyXMatineeCameraActor.h"
#include "Entities/XMatineeCameraActor.h"
#include "Entities/Primitive/SkeletalMesh/LegacySkeletalMeshComponent.h"

UClass* ULegacyXMatineeCameraActor::GetActorClass_Implementation()
{
	return AXMatineeCameraActor::StaticClass();
}


void ULegacyXMatineeCameraActor::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);

	AXMatineeCameraActor* CameraActor = CastChecked<AXMatineeCameraActor>(InActor);
	SkeletalMeshComponent->FillComponent(CameraActor->SkeletalMesh);
}
