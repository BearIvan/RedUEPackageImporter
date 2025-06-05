#pragma once

#include "Core/LegacyObject.h"
#include "LegacyLevel.h"
#include "LegacyWorld.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyWorld : public ULegacyObject
{
    GENERATED_BODY()

public:
    virtual void        LegacySerialize (FRedUELegacyArchive& Ar) override;
    virtual UObject*    ExportToContent () override;
            void        ImportLevel     (bool ReimportKismet = false);
    
    UFUNCTION(BlueprintCallable,CallInEditor,Category="Legacy")
    void ImportWorld(TSet<FName> AllowLevels,TSet<FName> DenyLevels,bool AllowAlwaysLoadingLevel,bool ImportPersistentLevel = true,bool ReimportKismet = false);

    UPROPERTY(BlueprintReadOnly)
    ULegacyLevel*PersistentLevel;

};


USTRUCT(Blueprintable)
struct FLegacyXWorldFloatingSectionIndexData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    int32 Index;
    
    UPROPERTY(BlueprintReadWrite)
    FGuid ActorGuid;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FName> LevelNames;
};


UCLASS()
class REDUELEGACY_API ULegacyXWorldFloatingSectionIndexTable: public ULegacyObject
{
    GENERATED_BODY()

public:    
    UPROPERTY(BlueprintReadWrite)
    TArray<FLegacyXWorldFloatingSectionIndexData> MapData;
};