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
    
    int32           LegacyPackageIndex = INDEX_NONE;
    int64			LegacyObjectFlags;
    int32			NetIndex;

	UFUNCTION(BlueprintCallable,Category="Legacy")
	virtual UObject* ExportToContent();
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FString GetLegacyFullName() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FString GetLegacyName() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FName GetLegacyFName() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FString GetOutContentPath() const;
	
    UFUNCTION(BlueprintNativeEvent)
    bool LegacySupport(ERedUELegacyEngineType EngineType,ERedUELegacyGameType GameType);
    
    UFUNCTION(BlueprintNativeEvent)
    FName GetLegacyClassName(ERedUELegacyEngineType EngineType,ERedUELegacyGameType GameType);
    
    UPROPERTY(Transient,BlueprintReadOnly,Category="Legacy")
    ULegacyPackage* LegacyPackage;

	UPROPERTY(Transient,BlueprintReadOnly,Category="Legacy")
	UObject* PresentObject = nullptr;
};
