#include "Core/RedUELegacySubsystem.h"

#include "Editor.h"
#include "EditorLevelUtils.h"
#include "LegacyTypeInfo.h"
#include "LevelUtils.h"
#include "PackageTools.h"
#include "RedUELegacyImporterSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/LegacyObject.h"
#include "Core/LegacyPackage.h"
#include "Entities/LegacyActor.h"
#include "Entities/LegacyActorComponent.h"
#include "Kismet/Base/SequenceAction.h"
#include "Material/SingularityTextureFileCache.h"
#include "Sequence/LegacySequenceObjects.h"
#include "Sounds/WWiseBanksManager.h"
#include "World/LegacyWorld.h"
DEFINE_LOG_CATEGORY(LogRedUELegacy);

URedUELegacySubsystem::URedUELegacySubsystem()
{
    SingularityTextureFileCache = CreateDefaultSubobject<USingularityTextureFileCache>("SingularityTextureFileCache");
    WWiseBanksManager = CreateDefaultSubobject<UWWiseBanksManager>("WWiseBanksManager");
    OutContentPath = TEXT("/Game");
}

void URedUELegacySubsystem::ObjectPreload(ULegacyObject* InObject) const
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
            if (InObject->LegacyObjectFlags & RLF_ClassDefaultObject)
            {
                InObject->LegacySerializeDefaultObject(*Package);
            }
            else
            {
                InObject->LegacySerialize(*Package);
            }
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
            if (InObject->LegacyObjectFlags & RLF_ClassDefaultObject)
            {
                InObject->LegacySerializeDefaultObject(*Package);
            }
            else
            {
                InObject->LegacySerialize(*Package);
            }
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
    
    for (FRedUELegacyExportPostLoad& ExportPostLoad :ExportsPostLoad)
    {
        if (ExportPostLoad.ArrayIndex != INDEX_NONE)
        {
            ExportPostLoad.ObjectProperty->SetObjectPropertyValue_InContainer(ExportPostLoad.To,ExportPostLoad.From->ExportToContent(),ExportPostLoad.ArrayIndex);
        }
        else
        {
            ExportPostLoad.ObjectProperty->SetObjectPropertyValue(ExportPostLoad.To,ExportPostLoad.From->ExportToContent());
        }
    }
    ExportsPostLoad.Empty();
    
    ensure(ObjectsLoaded.Num() == 0);
    ObjectsBeginLoadCount--;
}

void URedUELegacySubsystem::RefreshClasses(ERedUELegacyEngineType InCurrentEngineType, ERedUELegacyGameType InCurrentGameType)
{
    CurrentEngineType = InCurrentEngineType;
    CurrentGameType = InCurrentGameType;

    Classes.Empty();
    SequenceActionClasses.Empty();
    
    {
        FARFilter Filter;
        Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
        Filter.bRecursiveClasses = true;
        Filter.TagsAndValues.Add(FBlueprintTags::NativeParentClassPath, FObjectPropertyBase::GetExportPath(ULegacyActor::StaticClass()));

        const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

        TArray<FAssetData> AssetDataList;
        AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

        for (const FAssetData& InAssetData : AssetDataList)
        {
            int32 BPClassFlagsInt;
            InAssetData.GetTagValue(FBlueprintTags::ClassFlags, BPClassFlagsInt);

            if (!!(BPClassFlagsInt & static_cast<int32>( CLASS_Abstract | CLASS_HideDropDown | CLASS_Deprecated | CLASS_NewerVersionExists)))
            {
                continue;
            }
            if (!InAssetData.IsAssetLoaded())
            {
                InAssetData.GetAsset();
            }
        }
    }
    {
        FARFilter Filter;
        Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
        Filter.bRecursiveClasses = true;
        Filter.TagsAndValues.Add(FBlueprintTags::NativeParentClassPath, FObjectPropertyBase::GetExportPath(ULegacyActorComponent::StaticClass()));

        const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

        TArray<FAssetData> AssetDataList;
        AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

        for (const FAssetData& InAssetData : AssetDataList)
        {
            int32 BPClassFlagsInt;
            InAssetData.GetTagValue(FBlueprintTags::ClassFlags, BPClassFlagsInt);

            if (!!(BPClassFlagsInt & static_cast<int32>( CLASS_Abstract | CLASS_HideDropDown | CLASS_Deprecated | CLASS_NewerVersionExists)))
            {
                continue;
            }
            if (!InAssetData.IsAssetLoaded())
            {
                InAssetData.GetAsset();
            }
        }
    }
    TArray<UClass*> Libraries;
    GetDerivedClasses(ULegacyObject::StaticClass(), Libraries);
    Libraries.Add(ULegacyObject::StaticClass());
    for (UClass* LibraryClass : Libraries)
    {
        ULegacyObject* Factory = LibraryClass->GetDefaultObject<ULegacyObject>();
        if(Factory->LegacySupport(CurrentEngineType,CurrentGameType))
        {
            Classes.Add(Factory->GetLegacyClassName(CurrentEngineType,CurrentGameType),LibraryClass);
        }
        FName LegacyPackageName = *LibraryClass->GetMetaData(TEXT("LegacyPackage"));
        if (!LegacyPackageName.IsNone())
        {
            VirtualPackages.FindOrAdd(LegacyPackageName).Classes.Add(Factory->GetLegacyClassName(CurrentEngineType,CurrentGameType),LibraryClass);
        }
    }
    if (CurrentEngineType  == ERedUELegacyEngineType::UnrealEngine3)
    {
        TArray<UClass*> LibrariesActions;
        GetDerivedClasses(USequenceAction::StaticClass(), LibrariesActions);
        for (UClass* LibraryClass : LibrariesActions)
        {
            if (CurrentEngineType  == ERedUELegacyEngineType::UnrealEngine3)
            {
                if( LibraryClass->IsChildOf(USequenceAction::StaticClass()))
                {
                    FString ClassName = LibraryClass->GetName();
                    if(ClassName.StartsWith(TEXT("Legacy")))
                    {
                        ClassName.RemoveAt(0,6);
                    }
                    SequenceActionClasses.Add(*ClassName, LibraryClass);
                }
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
        GetPackage(TEXT("Engine"));
        GetPackage(TEXT("Startup_INT"));
    }
    if(CurrentGameType == ERedUELegacyGameType::Singularity)
    {
        GetPackage(TEXT("RvGame"));
    }
    if(CurrentGameType == ERedUELegacyGameType::Bioshock3)
    {
        GetPackage(TEXT("XCore"));
        GetPackage(TEXT("Master_P"));
        GetPackage(TEXT("XEntry_p"));
        GetPackage(TEXT("DLCB_Master_P"));
        GetPackage(TEXT("dlcb_CoalescedItems"));
    }
}


ULegacyObject* URedUELegacySubsystem::CreateObject(FName ObjectName, FName ClassName, ULegacyPackage* FromPackage,UObject* Outer, EObjectFlags InObjectFlags)
{
    ensure(CurrentGameType == FromPackage->GameType);
    ensure(CurrentEngineType == FromPackage->EngineType);
    
    if (!Outer)
    {
        Outer = FromPackage;
    }
    const TSubclassOf<ULegacyObject>* Class = Classes.Find(ClassName);
    if (!Class && SequenceActionClasses.Contains(ClassName))
    {
        ULegacySequenceImporter* SequenceImporter = NewObject<ULegacySequenceImporter>(Outer,ULegacySequenceImporter::StaticClass(),ObjectName,InObjectFlags);
        if (ensure(SequenceImporter))
        {
            static FName  NAME_Action = "Action";
            SequenceImporter->ToAction = NewObject<USequenceAction>(SequenceImporter,SequenceActionClasses[ClassName],NAME_Action, InObjectFlags|RF_DefaultSubObject);
            return SequenceImporter;
        }
        return nullptr;
    }
    if(!Class)
    {
        return nullptr;
    }
    ULegacyObject*Result = NewObject<ULegacyObject>(Outer,Class->Get(),ObjectName,InObjectFlags);
    ensure(Result);
    return Result;
}

bool URedUELegacySubsystem::IsKnownClass(FName ClassName)
{
    const TSubclassOf<ULegacyObject>* Class = Classes.Find(ClassName);
    return Class != nullptr || SequenceActionClasses.Contains(ClassName);
}

UCurveFloat* URedUELegacySubsystem::GetBrightnessToEVCurve()
{
    if (!BrightnessToEV)
    {
        BrightnessToEV = LoadObject<UCurveFloat>(nullptr, TEXT("/RedUEPackageImporter/CF_BrightnessToEV.CF_BrightnessToEV"));
    }
    return BrightnessToEV;
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
    
    auto LambdaLoadPackage = [this](const FString&FileName)->ULegacyPackage*
    {
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
    };
    if (ULegacyPackage* Package = LambdaLoadPackage(FileName))
    {
        if (Package->GameType == ERedUELegacyGameType::Singularity)
        {
            LambdaLoadPackage(FPaths::GetBaseFilename(FileName,false) + TEXT("_XSA"));
            LambdaLoadPackage(FPaths::GetBaseFilename(FileName,false) + TEXT("_LOC_") + GetDefault<URedUELegacyImporterSettings>()->Language.ToUpper());
        }
        return Package;
    }
    return nullptr;
}

void URedUELegacySubsystem::Clear()
{
    OptionalContentPaths.Empty();
    Packages.Empty();
    ObjectsLoaded.Empty();
    Classes.Empty();
    SequenceActionClasses.Empty();
    CacheNoFoundClasses.Empty();
    Skeletons.Empty();
    WWiseBanksManager->Empty();
    SingularityTextureFileCache->Empty();
    CurrentEngineType = ERedUELegacyEngineType::Unkown;
    CurrentGameType = ERedUELegacyGameType::Unkown;
    FloatingSectionIndexTable = nullptr;
    GLegacyWorld = nullptr;
    CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}

void URedUELegacySubsystem::ToCacheSkeletons()
{
    Skeletons.Empty();
    auto ScanLambda = [this](const FString&Path)
    {
        const FString PackageName = UPackageTools::SanitizePackageName(Path);
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
    };
    ScanLambda(OutContentPath);
    for (const FString& OptionalContentPath : OptionalContentPaths)
    {
        ScanLambda(OptionalContentPath);
    }
}
