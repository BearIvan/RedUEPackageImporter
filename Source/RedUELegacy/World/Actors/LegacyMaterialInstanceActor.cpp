#include "World/Actors/LegacyMaterialInstanceActor.h"

#include "Actors/MaterialInstanceActor/MaterialInstanceHybridActor.h"
#include "Material/LegacyMaterialInstance.h"
#include "Materials/Hybrid/MaterialInstanceHybrid.h"

void ULegacyMaterialInstanceActor::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	AMaterialInstanceHybridActor* RedMaterialInstance = CastChecked<AMaterialInstanceHybridActor>(InActor);
	RedMaterialInstance->MaterialInstance = Cast<UMaterialInstanceHybrid>(MatInst->ExportToContent());
}

UClass* ULegacyMaterialInstanceActor::GetActorClass_Implementation()
{
	return AMaterialInstanceHybridActor::StaticClass();
}
