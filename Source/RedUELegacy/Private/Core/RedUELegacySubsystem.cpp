﻿// Tyran


#include "Core/RedUELegacySubsystem.h"

#include "Editor.h"
#include "EditorLevelUtils.h"
#include "LevelUtils.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/LegacyObject.h"
#include "Core/LegacyPackage.h"
#include "Kismet/Base/SequenceAction.h"
#include "Material/SingularityTextureFileCache.h"
#include "World/LegacyWorld.h"
#include "World/Sequences/Object/LegacySequenceObjects.h"
DEFINE_LOG_CATEGORY(LogRedUELegacy);

URedUELegacySubsystem::URedUELegacySubsystem()
{
    SingularityTextureFileCache = CreateDefaultSubobject<USingularityTextureFileCache>("SingularityTextureFileCache");
    OutContentPath = TEXT("/Game");
}

void URedUELegacySubsystem::ObjectPreload(ULegacyObject* InObject)
{
    if ( ObjectsLoaded.Contains(InObject))
    {
        if (InObject->LegacyObjectFlags & RLF_NeedLoad)
        {
            InObject->LegacyObjectFlags &= ~RLF_NeedLoad;
            ULegacyPackage *Package = InObject->LegacyPackage;
            const INT SavedPos = Package->Tell();
            Package->PushStopper();
            Package->SetupReader(InObject->LegacyPackageIndex);
            UE_LOG(LogRedUELegacy,Log,TEXT("Pre loading %s %s from package %s\n"), *InObject->GetClass()->GetName(), *InObject->GetLegacyFullName(), *Package->FileName);
            InObject->LegacySerialize(*Package);
            if (Package->GetStopper() != Package->Tell())
            {
                UE_LOG(LogRedUELegacy,Warning,TEXT("%s::LegacySerialize(%s): %lld unread bytes"),*InObject->GetClass()->GetName(), *InObject->GetLegacyFullName(), Package->GetStopper() - Package->Tell());
            }
            Package->Seek(SavedPos);
            Package->PopStopper();
        }
    }
}

void URedUELegacySubsystem::ObjectsBeginLoad()
{
    check(ObjectsBeginLoadCount >= 0);
    ObjectsBeginLoadCount++;
}

void URedUELegacySubsystem::ObjectsEndLoad()
{
    
    check(ObjectsBeginLoadCount > 0);
    if (ObjectsBeginLoadCount > 1)
    {
        ObjectsBeginLoadCount--;
        return;
    }
    TArray<ULegacyObject*> LoadedObjects;
    while(ObjectsLoaded.Num() > 0)
    {
        ULegacyObject *InObject = ObjectsLoaded[0];
        ObjectsLoaded.RemoveAt(0);
        if (InObject->LegacyObjectFlags &RLF_NeedLoad)
        {
            InObject->LegacyObjectFlags &= ~RLF_NeedLoad;
            ULegacyPackage *Package = InObject->LegacyPackage;
            Package->SetupReader(InObject->LegacyPackageIndex);
            UE_LOG(LogRedUELegacy,Log,TEXT("Loading %s %s from package %s\n"), *InObject->GetClass()->GetName(), *InObject->GetLegacyFullName(), *Package->FileName);
            InObject->LegacySerialize(*Package);
            if (Package->GetStopper() != Package->Tell())
            {
                UE_LOG(LogRedUELegacy,Warning,TEXT("%s::LegacySerialize(%s): %lld unread bytes"),*InObject->GetClass()->GetName(), *InObject->GetLegacyFullName(), Package->GetStopper() - Package->Tell());
            }
            LoadedObjects.Add(InObject);
        }
		
    }
    
    for (ULegacyObject*LoadedObject:LoadedObjects)
    {
        LoadedObject->LegacyPostLoad();
    }
    
    ensure(ObjectsLoaded.Num() == 0);
    ObjectsBeginLoadCount--;
}

void URedUELegacySubsystem::RefreshClasses(ERedUELegacyEngineType InCurrentEngineType, ERedUELegacyGameType InCurrentGameType)
{
    CurrentEngineType = InCurrentEngineType;
    CurrentGameType = InCurrentGameType;

    Classes.Empty();
    SequenceActionClasses.Empty();
    
    for(TObjectIterator<UClass> It; It; ++It)
    {
        if( It->IsChildOf(ULegacyObject::StaticClass()))
        {
            ULegacyObject* Factory = It->GetDefaultObject<ULegacyObject>();
            if(Factory->LegacySupport(CurrentEngineType,CurrentGameType))
            {
                Classes.Add(Factory->GetLegacyClassName(CurrentEngineType,CurrentGameType),*It);
            }
        }
        if (CurrentEngineType  == ERedUELegacyEngineType::UnrealEngine3)
        {
            if( It->IsChildOf(USequenceAction::StaticClass()))
            {
                FString ClassName = It->GetName();
                if(ClassName.StartsWith(TEXT("Legacy")))
                {
                    ClassName.RemoveAt(0,6);
                }
                SequenceActionClasses.Add(*ClassName,*It);
            }
        }
     
    }
}

void URedUELegacySubsystem::Initialize(ERedUELegacyEngineType InCurrentEngineType, ERedUELegacyGameType InCurrentGameType)
{
    Packages.Empty();
    CacheNoFoundClasses.Empty();
    RefreshClasses(InCurrentEngineType,InCurrentGameType);
    if(CurrentEngineType==ERedUELegacyEngineType::UnrealEngine3)
    {
        GetPackage(TEXT("Startup_INT"));
    }
    if(CurrentGameType == ERedUELegacyGameType::Singularity)
    {
        GetPackage(TEXT("RvGame"));
    }
    if(CurrentGameType == ERedUELegacyGameType::Bioshock3)
    {
        GetPackage(TEXT("Master_P"));
        GetPackage(TEXT("XEntry_p"));
        GetPackage(TEXT("DLCB_Master_P"));
        GetPackage(TEXT("dlcb_CoalescedItems"));
    }
}


ULegacyObject* URedUELegacySubsystem::CreateObject(FName ObjectName, FName ClassName, ULegacyPackage* FromPackage)
{
    ensure(CurrentGameType == FromPackage->GameType);
    ensure(CurrentEngineType == FromPackage->EngineType);
    
    const TSubclassOf<ULegacyObject>* Class = Classes.Find(ClassName);
    if (!Class && SequenceActionClasses.Contains(ClassName))
    {
        ULegacySequenceImporter* SequenceImporter = NewObject<ULegacySequenceImporter>(FromPackage,ULegacySequenceImporter::StaticClass(),ObjectName);
        SequenceImporter->ToAction = NewObject<USequenceAction>(SequenceImporter,SequenceActionClasses[ClassName],ObjectName,RF_NoFlags);
        return SequenceImporter;
    }
    if(!Class)
    {
        return nullptr;
    }
    ULegacyObject*Result = NewObject<ULegacyObject>(FromPackage,Class->Get(),ObjectName);
    return Result;
}

bool URedUELegacySubsystem::IsKnownClass(FName ClassName)
{
    const TSubclassOf<ULegacyObject>* Class = Classes.Find(ClassName);
    return Class != nullptr || SequenceActionClasses.Contains(ClassName);
}

void URedUELegacySubsystem::ImportWorld(FName PackageName, TSet<FName> AllowLevels, TSet<FName> DenyLevels, bool AllowAlwaysLoadingLevel,bool ImportPersistentLevel, bool ReimportKismet)
{

    if (ULegacyPackage *Package = GetPackage(PackageName.ToString()))
    {
        if (CurrentGameType == ERedUELegacyGameType::Bioshock3)
        {
            FloatingSectionIndexTable =  nullptr;
            if (ULegacyPackage *FloatingSectionPackage = GetPackage(PackageName.ToString() + TEXT("_FWIT_SF")))
            {
                int32 XWorldFloatingSectionIndexTableIndex = FloatingSectionPackage->FindExport("XWorldFloatingSectionIndexTable");
                if(XWorldFloatingSectionIndexTableIndex!=INDEX_NONE)
                {
                    FloatingSectionIndexTable = Cast<ULegacyXWorldFloatingSectionIndexTable>( FloatingSectionPackage->GetOrCreateExport(XWorldFloatingSectionIndexTableIndex));
                }
            }
        }
        
        int32 TheWorldIndex = Package->FindExport(NAME_TheWorld);
        if(TheWorldIndex!=INDEX_NONE)
        {
            if(ULegacyWorld* LegacyWorld = Cast<ULegacyWorld>( Package->GetOrCreateExport(TheWorldIndex)))
            {
                GLegacyWorld = LegacyWorld;
                LegacyWorld->ImportWorld(AllowLevels,DenyLevels,AllowAlwaysLoadingLevel,ImportPersistentLevel,ReimportKismet);
            }
        }
    }
}

ULegacyPackage* URedUELegacySubsystem::GetPackage(const FString& FileName)
{
    if(ULegacyPackage**Package =  Packages.Find(FileName))
    {
        return *Package;
    }
    
    const FString Extensions[] = {TEXT(".xxx"),TEXT(".upk")};
    for(const FString&Extension:Extensions)
    {
        for (const FString&InContentPath:InContentPaths)
        {
            if(FPaths::FileExists(FPaths::Combine(InContentPath,FileName+Extension)))
            {
                ULegacyPackage *NewPackage  = NewObject<ULegacyPackage>(this,ULegacyPackage::StaticClass(),*FileName);
                NewPackage->LoadPackage(*(FileName+Extension));
                if(CurrentEngineType==ERedUELegacyEngineType::Unkown)
                {
                    Initialize(NewPackage->EngineType,NewPackage->GameType);
                }
                else if(NewPackage->EngineType!=CurrentEngineType||NewPackage->GameType!=CurrentGameType)
                {
                    ensure(false);
                    NewPackage->MarkAsGarbage();
                    return nullptr;
                }
                Packages.Add(FileName,NewPackage);
                return NewPackage;
            }
        }
    }
    return nullptr;
}

void URedUELegacySubsystem::Clear()
{
    Packages.Empty();
    ObjectsLoaded.Empty();
    Classes.Empty();
    SequenceActionClasses.Empty();
    CacheNoFoundClasses.Empty();
    Skeletons.Empty();
    CurrentEngineType = ERedUELegacyEngineType::Unkown;
    CurrentGameType = ERedUELegacyGameType::Unkown;
    FloatingSectionIndexTable = nullptr;
    GLegacyWorld = nullptr;
    CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}

void URedUELegacySubsystem::ToCacheSkeletons()
{
    Skeletons.Empty();
    const FString PackageName = UPackageTools::SanitizePackageName(OutContentPath);
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> AssetData;
    AssetRegistryModule.Get().GetAssetsByPath(FName(*PackageName), AssetData, true);
    for (FAssetData& Data : AssetData)
    {
        if (USkeleton* Skeleton = Cast<USkeleton>(Data.GetAsset()))
        {
            Skeletons.Add(Skeleton);
        }
    }
}
