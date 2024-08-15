#include "World/Actors/LegacyMaterialInstanceActor.h"

#include "Actors/MaterialInstanceActor/RedMaterialInstanceActor.h"
#include "Material/LegacyMaterialInstance.h"

void ULegacyMaterialInstanceActor::Spawn_Implementation()
{
	ARedMaterialInstanceActor* RedMaterialInstanceActor = GWorld->SpawnActor<ARedMaterialInstanceActor>(FVector(Translation),FRotator(Rotation));
	RedMaterialInstanceActor->SetActorScale3D(FVector(Scale3D));
	RedMaterialInstanceActor->MaterialInstance = Cast<UMaterialInstanceConstant>(MatInst->ExportToContent());
	PresentObject = RedMaterialInstanceActor;
}
