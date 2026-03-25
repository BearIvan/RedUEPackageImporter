#pragma once

#include "Actors/LegacyActor.h"
#include "Containers/TransArray.h"
#include "Core/LegacyObject.h"
#include "LegacyLevel.generated.h"

class ULegacyModel;
class ULegacySequence;

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyLevel : public ULegacyObject
{
    GENERATED_BODY()

public:
    virtual void        LegacySerialize(FRedUELegacyArchive& Ar) override;
    virtual UObject*    ExportToContent() override;
	        void        ImportLevel     (bool ReimportKismet = false);
            FURL		URL;
    
    UPROPERTY()
    TArray<ULegacyActor*> Actors;

    UPROPERTY()
    ULegacyModel * Model;
    
    UPROPERTY()
    TArray<UObject*> ModelComponents;
    
    UPROPERTY()
    TArray<ULegacySequence*> GameSequences;
    
    UPROPERTY(Transient)
    ULegacyWorldInfo* WorldInfo;
};
