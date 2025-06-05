// Tyran

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
UCLASS(Transient,BlueprintType)
class REDUELEGACY_API URedUELegacySubsystem : public UEditorSubsystem
{
    GENERATED_BODY()

public:
                    URedUELegacySubsystem   ();
    void            ObjectPreload			(ULegacyObject* Object);
    void            ObjectsBeginLoad        ();
    void            ObjectsEndLoad          ();
    void            RefreshClasses          (ERedUELegacyEngineType CurrentEngineType, ERedUELegacyGameType CurrentGameType);
    void            Initialize				(ERedUELegacyEngineType CurrentEngineType, ERedUELegacyGameType CurrentGameType);
    ULegacyObject*  CreateObject            (FName ObjectName,FName ClassName, ULegacyPackage*FromPackage);
    bool            IsKnownClass            (FName ClassName);
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
	TMap<FName,TSubclassOf<class USequenceAction>> SequenceActionClasses;

	UPROPERTY(Transient)
	class USingularityTextureFileCache* SingularityTextureFileCache = nullptr;
	
	UPROPERTY(Transient,BlueprintReadWrite)
	TArray<FString> InContentPaths;
	
	UPROPERTY(Transient,BlueprintReadWrite)
	FString OutContentPath;

	UPROPERTY(Transient)
	TSet<FName> CacheNoFoundClasses;
	
	UPROPERTY(Transient)
	TArray<USkeleton*> Skeletons;

	UPROPERTY(Transient)
	ULegacyXWorldFloatingSectionIndexTable* FloatingSectionIndexTable = nullptr;
	
    ERedUELegacyGameType CurrentGameType = ERedUELegacyGameType::Unkown;
    ERedUELegacyEngineType CurrentEngineType = ERedUELegacyEngineType::Unkown;
    
};
