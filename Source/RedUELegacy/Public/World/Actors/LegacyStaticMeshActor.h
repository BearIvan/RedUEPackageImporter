#pragma once

#include "LegacyActor.h"
#include "LegacyStaticMeshActor.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyStaticMeshActor : public ULegacyActor
{
    GENERATED_BODY()
public:
    virtual void Spawn_Implementation() override;
    
    UPROPERTY(BlueprintReadWrite)
    class ULegacyStaticMeshComponent* StaticMeshComponent;

};
