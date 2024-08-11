#pragma once

#include "Core/LegacyObject.h"
#include "LegacyLevel.h"
#include "LegacyWorld.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyWorld : public ULegacyObject
{
    GENERATED_BODY()

public:
    virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;
    virtual UObject* ExportToContent() override;

    UPROPERTY(BlueprintReadOnly)
    ULegacyLevel*PersistentLevel;
};
