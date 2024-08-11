#pragma once
#include "LegacyObject.generated.h"


class ULegacyPackage;
class FRedUELegacyArchive;

UCLASS()
class REDUELEGACY_API ULegacyObject : public UObject
{
	GENERATED_BODY()
public:
    virtual void        LegacyPostLoad              ();
    virtual void        LegacySerialize             (FRedUELegacyArchive& Ar);
	virtual void        LegacySerializeComponent    (FArchive& Ar) { }
    virtual bool        IsAComponent                () { return false; }
    virtual void        LegacySerializeUnrealProps  (UStruct* Type, void*Object,FRedUELegacyArchive& Ar);
    virtual UObject*    ExportToContent             ();
            FString     GetLegacyFullName           () const;
    
    int32           LegacyPackageIndex = INDEX_NONE;
    int64			LegacyObjectFlags;
    int32			NetIndex;

	UFUNCTION(BlueprintCallable)
	FString GetOutContentPath() const;
	
    UFUNCTION(BlueprintNativeEvent)
    bool LegacySupport(ERedUELegacyEngineType EngineType,ERedUELegacyGameType GameType);
    
    UFUNCTION(BlueprintNativeEvent)
    FName GetLegacyClassName(ERedUELegacyEngineType EngineType,ERedUELegacyGameType GameType);
    
    UPROPERTY(Transient,BlueprintReadOnly)
    ULegacyPackage* LegacyPackage;

	UPROPERTY(Transient,BlueprintReadOnly)
	UObject* PresentObject = nullptr;
};
