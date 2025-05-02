#include "World/Actors/LegacyMaterialInstanceActor.h"

#include "Actors/MaterialInstanceActor/RedMaterialInstanceActor.h"
#include "Material/LegacyMaterialInstance.h"

void ULegacyMaterialInstanceActor::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	ARedMaterialInstanceActor* RedMaterialInstance = CastChecked<ARedMaterialInstanceActor>(InActor);
	RedMaterialInstance->MaterialInstance = Cast<UMaterialInstanceConstant>(MatInst->ExportToContent());
}

UClass* ULegacyMaterialInstanceActor::GetActorClass_Implementation()
{
	return ARedMaterialInstanceActor::StaticClass();
}
