// Tyran


#include "Core/RedUELegacySubsystem.h"

#include "Core/LegacyObject.h"
#include "Core/LegacyPackage.h"
#include "Material/SingularityTextureFileCache.h"
DEFINE_LOG_CATEGORY(LogRedUELegacy);

URedUELegacySubsystem::URedUELegacySubsystem()
{
    SingularityTextureFileCache = CreateDefaultSubobject<USingularityTextureFileCache>("SingularityTextureFileCache");
    OutContentPath = TEXT("/Game");
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
		ULegacyPackage *Package = InObject->LegacyPackage;
        Package->SetupReader(InObject->LegacyPackageIndex);
        UE_LOG(LogRedUELegacy,Log,TEXT("Loading %s %s from package %s\n"), *InObject->GetClass()->GetName(), *InObject->GetName(), *Package->FileName);
        InObject->LegacySerialize(*Package);
        if (Package->GetStopper() != Package->Tell())
        {
            UE_LOG(LogRedUELegacy,Warning,TEXT("%s::LegacySerialize(%s): %lld unread bytes"),*InObject->GetClass()->GetName(), *InObject->GetName(), Package->GetStopper() - Package->Tell());
        }
         
    }
    
    for (ULegacyObject*LoadedObject:LoadedObjects)
    {
        LoadedObject->PostLoad();
    }
    
    ensure(ObjectsLoaded.Num() == 0);
    ObjectsBeginLoadCount--;
}

void URedUELegacySubsystem::RefreshClasses(ERedUELegacyEngineType InCurrentEngineType, ERedUELegacyGameType InCurrentGameType)
{
    CurrentEngineType = InCurrentEngineType;
    CurrentGameType = InCurrentGameType;

    Classes.Empty();
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
}


ULegacyObject* URedUELegacySubsystem::CreateObject(FName ObjectName, FName ClassName, ULegacyPackage* FromPackage)
{
    ensure(CurrentGameType == FromPackage->GameType);
    ensure(CurrentEngineType == FromPackage->EngineType);
    
    const TSubclassOf<ULegacyObject>* Class = Classes.Find(ClassName);
    if(!Class)
    {
        return nullptr;
    }
    return NewObject<ULegacyObject>(FromPackage,Class->Get(),ObjectName);
}

bool URedUELegacySubsystem::IsKnownClass(FName ClassName)
{
    const TSubclassOf<ULegacyObject>* Class = Classes.Find(ClassName);
    return Class != nullptr;
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
    return nullptr;
}
