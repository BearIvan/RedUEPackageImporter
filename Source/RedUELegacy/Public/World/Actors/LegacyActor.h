#pragma once

#include "Core/LegacyObject.h"
#include "LegacyActor.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyActor : public ULegacyObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent)
    void Spawn();
};
