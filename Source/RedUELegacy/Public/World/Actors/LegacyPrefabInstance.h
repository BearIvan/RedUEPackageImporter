#pragma once

#include "LegacyActor.h"
#include "LegacyPrefabInstance.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyPrefabInstance : public ULegacyActor
{
    GENERATED_BODY()

public:
    virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;
    virtual AActor* Spawn_Implementation() override;
    virtual void LegacyPostLoad() override;

    UPROPERTY(BlueprintReadWrite)
    class ULegacyPrefab* TemplatePrefab;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<ULegacyActor*,ULegacyActor* > ArchetypeToInstanceMap;
};
