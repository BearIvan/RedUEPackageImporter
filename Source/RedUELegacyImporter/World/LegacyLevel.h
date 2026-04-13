#pragma once

#include "Core/LegacyObject.h"
#include "LegacyLevel.generated.h"

class ULegacyModel;
class ULegacySequence;
class ULegacyWorldInfo;
class ULegacyActor;

UCLASS(meta = (LegacyPackage = Engine))
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
