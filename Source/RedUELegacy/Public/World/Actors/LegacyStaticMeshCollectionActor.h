#pragma once

#include "LegacyActor.h"
#include "Core/LegacyObject.h"
#include "World/Components/LegacyStaticMeshComponent.h"
#include "LegacyStaticMeshCollectionActor.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyStaticMeshCollectionActor : public ULegacyActor
{
    GENERATED_BODY()

public:
    virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;
    virtual AActor* Spawn_Implementation() override;

    UPROPERTY(BlueprintReadWrite)
    TArray<	ULegacyStaticMeshComponent*> StaticMeshComponents;

};
