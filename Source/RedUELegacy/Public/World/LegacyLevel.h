#pragma once

#include "Actors/LegacyActor.h"
#include "Containers/TransArray.h"
#include "Core/LegacyObject.h"
#include "LegacyLevel.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyLevel : public ULegacyObject
{
    GENERATED_BODY()

public:
    virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;
    virtual UObject* ExportToContent() override;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<ULegacyActor*> Actors;
};
