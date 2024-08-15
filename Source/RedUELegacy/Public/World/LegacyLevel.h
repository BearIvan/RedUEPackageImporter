#pragma once

#include "Actors/LegacyActor.h"
#include "Containers/TransArray.h"
#include "Core/LegacyObject.h"
#include "LegacyLevel.generated.h"

class ULegacySequence;

UCLASS()
class REDUELEGACY_API ULegacyLevel : public ULegacyObject
{
    GENERATED_BODY()

public:
    virtual void        LegacySerialize(FRedUELegacyArchive& Ar) override;
    virtual UObject*    ExportToContent() override;
	
            FURL		URL;
    
    UPROPERTY()
    TArray<ULegacyActor*> Actors;

    UPROPERTY()
    UObject * Model;
    
    UPROPERTY()
    TArray<UObject*> ModelComponents;
    
    UPROPERTY()
    TArray<ULegacySequence*> GameSequences;
};
