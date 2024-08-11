#pragma once

#include "LegacyActorComponent.h"
#include "Material/LegacyMaterialInterface.h"
#include "LegacyStaticMeshComponent.generated.h"


UCLASS()
class REDUELEGACY_API ULegacyStaticMeshComponent : public ULegacyActorComponent
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    ULegacyObject* StaticMesh;

    UPROPERTY(BlueprintReadWrite)
    TArray<ULegacyMaterialInterface*>	Materials;
};
