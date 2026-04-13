#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "LegacyObject.h"
#include "RedUELegacyGame.h"
#include "RedUELegacySubsystem.generated.h"

class ULegacyXWorldFloatingSectionIndexTable;
class ULegacyPackage;
class ULegacyObject;
class ULegacyWorld;
/**
 * 
 */
USTRUCT()
struct FRedUELegacyVirtualPackage
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<FName,TSubclassOf<ULegacyObject>> Classes;
};

USTRUCT()
struct FRedUELegacyExportPostLoad
{
	GENERATED_BODY()
	
	UPROPERTY()
	ULegacyObject* From;
	
	void* To;
	FObjectProperty* ObjectProperty;
	int32 ArrayIndex = 0;
};

UCLASS(Transient,BlueprintType)
class REDUELEGACYIMPORTER_API URedUELegacySubsystem : public UEditorSubsystem
{
    GENERATED_BODY()

public:
                    URedUELegacySubsystem   ();
    void            ObjectPreload			(ULegacyObject* Object) const;
    void            ObjectsBeginLoad        ();
    void            ObjectsEndLoad          ();
    void            RefreshClasses          (ERedUELegacyEngineType CurrentEngineType, ERedUELegacyGameType CurrentGameType);
    void            Initialize				(ERedUELegacyEngineType CurrentEngineType, ERedUELegacyGameType CurrentGameType);
    ULegacyObject*  CreateObject            (FName ObjectName,FName ClassName, ULegacyPackage*FromPackage,UObject* Outer = nullptr, EObjectFlags ObjectFlags = RF_Public);
    bool            IsKnownClass            (FName ClassName);
	UCurveFloat*	GetBrightnessToEVCurve	();
    int32           ObjectsBeginLoadCount = 0;

	UFUNCTION(BlueprintCallable)
	void ImportWorld(FName PackageName,TSet<FName> AllowLevels, TSet<FName> DenyLevels, bool AllowAlwaysLoadingLevel,bool ImportPersistentLevel = true,bool ReimportKismet = false);
	
	UFUNCTION(BlueprintCallable)
	ULegacyPackage*	GetPackage(const FString&Name);
    
	UFUNCTION(BlueprintCallable)
	void Clear();
	
	UFUNCTION(BlueprintCallable)
	void ToCacheSkeletons();

	UPROPERTY(Transient)
	ULegacyWorld* GLegacyWorld = nullptr;
	
    UPROPERTY(Transient)
    TMap<FString,ULegacyPackage*> Packages;
    
    UPROPERTY(Transient)
	TArray<ULegacyObject*> ObjectsLoaded;
	
    UPROPERTY(Transient)
    TMap<FName,TSubclassOf<ULegacyObject>> Classes;
	
	UPROPERTY(Transient)
	TMap<FName,FRedUELegacyVirtualPackage> VirtualPackages;
	
	UPROPERTY(Transient)
	TMap<FName,TSubclassOf<class USequenceAction>> SequenceActionClasses;

	UPROPERTY(Transient)
	class USingularityTextureFileCache* SingularityTextureFileCache = nullptr;

	UPROPERTY(Transient)
	class UWWiseBanksManager* WWiseBanksManager = nullptr;
	
	UPROPERTY(Transient,BlueprintReadWrite)
	TArray<FString> InContentPaths;
	
	UPROPERTY(Transient,BlueprintReadWrite)
	TArray<FString> OptionalContentPaths;
	
	UPROPERTY(Transient,BlueprintReadWrite)
	FString OutContentPath;

	UPROPERTY(Transient)
	TSet<FName> CacheNoFoundClasses;
	
	UPROPERTY(Transient)
	TArray<USkeleton*> Skeletons;
	
	UPROPERTY(Transient)
	UCurveFloat* BrightnessToEV;
	
	UPROPERTY(Transient)
	ULegacyXWorldFloatingSectionIndexTable* FloatingSectionIndexTable = nullptr;
	
	UPROPERTY(Transient)
	TArray<FRedUELegacyExportPostLoad> ExportsPostLoad;
	
    ERedUELegacyGameType CurrentGameType = ERedUELegacyGameType::Unkown;
    ERedUELegacyEngineType CurrentEngineType = ERedUELegacyEngineType::Unkown;
    
};
