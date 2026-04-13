#pragma once

#include "Entities/Primitive/LegacyPrimitiveComponent.h"
#include "Material/LegacyMaterialInterface.h"
#include "LegacyStaticMeshComponent.generated.h"


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyStaticMeshComponent : public ULegacyMeshComponent
{
    GENERATED_BODY()
public:
    ULegacyStaticMeshComponent();
    
    virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;
    
    UPROPERTY(BlueprintReadWrite)
    ULegacyObject* StaticMesh;

    UPROPERTY(BlueprintReadWrite)
    TArray<ULegacyMaterialInterface*>	Materials;
};
