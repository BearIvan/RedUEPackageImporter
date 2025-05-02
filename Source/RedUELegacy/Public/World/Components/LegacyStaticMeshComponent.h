#pragma once

#include "LegacyActorComponent.h"
#include "LegacyPrimitiveComponent.h"
#include "Material/LegacyMaterialInterface.h"
#include "LegacyStaticMeshComponent.generated.h"


UCLASS()
class REDUELEGACY_API ULegacyStaticMeshComponent : public ULegacyPrimitiveComponent
{
    GENERATED_BODY()
public:
    virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;
    
    UPROPERTY(BlueprintReadWrite)
    ULegacyObject* StaticMesh;

    UPROPERTY(BlueprintReadWrite)
    TArray<ULegacyMaterialInterface*>	Materials;
};
