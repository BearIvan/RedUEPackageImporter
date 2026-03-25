#pragma once

#include "Core/LegacyObject.h"
#include "LegacyPrefab.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyPrefab : public ULegacyObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    TArray<class ULegacyActor*> PrefabArchetypes;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<class ULegacyActor*> RemovedArchetypes;

};
