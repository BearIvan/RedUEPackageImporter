#include "LegacyTypeInfo.h"

#include "RedUELegacySubsystem.h"
#include "Core/RedUELegacyArchive.h"

void ULegacyField::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    if (Ar.LegacyVer < 756 && Ar.Game != ERedUELegacyGame::Bioshock3)
        Ar << SuperField2;
    Ar << Next;
}

void ULegacyEnum::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
	Ar << Names;
}

void ULegacyConst::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
	Ar << Value;
}


void ULegacyFunction::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar.Seek(Ar.GetStopper());
}

void ULegacyTextBuffer::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
	Ar << Pos << Top << Text;
}

void ULegacyStruct::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    
    if (Ar.LegacyVer >= 756 || Ar.Game == ERedUELegacyGame::Bioshock3)
    {
        Ar << SuperField;
    }
    else
    {
        SuperField = SuperField2;
    }
    
    Ar << ScriptText;
    Ar << Children;
    Ar << CppText;
    
    Ar << Line << TextPos;
    int32 ScriptSize;
    Ar << ScriptSize;
    if (Ar.LegacyVer >= 639)
    {
        int32 ScriptStorageSize;
        Ar << ScriptStorageSize;
        Script.Empty( ScriptStorageSize );
        Script.Add( ScriptStorageSize );
        Ar.Serialize(&Script[0], ScriptStorageSize);

    }
    else  if (ScriptSize)
    {
        int remaining = Ar.GetStopper() - Ar.Tell();
        UE_LOG(LogRedUELegacy,Log,TEXT("script: %d, rest: %d\n"), ScriptSize, remaining);
        if (remaining < ScriptSize)
        {
            UE_LOG(LogRedUELegacy,Warning,TEXT("WARNING: bad ScriptSize, dropping\n"));
            return;
        }
        Script.Empty(ScriptSize);
        Script.Add(ScriptSize);
        Ar.Serialize(&Script[0], ScriptSize);
    }
}

void ULegacyScriptStruct::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar.Seek(Ar.GetStopper());
}

void ULegacyState::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    if (Ar.Game == ERedUELegacyGame::Singularity)
    {
        Ar << ProbeMask;
        Ar << IgnoreMask;
    }
    else
    {
        int32 ProbeMask32 = ProbeMask;
        Ar << ProbeMask32;
        ProbeMask = ProbeMask32;
    }
    Ar << LabelTableOffset << StateFlags;
    Ar << FuncMap;
}

FArchive& operator<<(FArchive& Ar, FLegacyImplementedInterface& R)
{
    Ar << R.Class << R.PointerProperty;
    return Ar;
}

void ULegacyClass::LegacySerialize(FRedUELegacyArchive& Ar)
{
 
    Super::LegacySerialize(Ar);
    
    Ar << ClassFlags;
    Ar << ClassWithin << ClassConfigName;
    Ar << ComponentNameToDefaultObjectMap;
    Ar << Interfaces;
    
    {
        if( Ar.LegacyVer >= 603 )
        {
            Ar << DontSortCategories;
        }
        
        Ar << HideCategories << AutoExpandCategories << AutoCollapseCategories;
        
        if( Ar.LegacyVer  >= 749 )
        {
            Ar << bForceScriptOrder;
        }
        else
        {
            bForceScriptOrder = 0;
        }

        if( Ar.LegacyVer >= 789 )
        {
            Ar << ClassGroupNames;
        }
        if (Ar.Game != ERedUELegacyGame::Bioshock3 && Ar.Game != ERedUELegacyGame::Singularity)
        {
            Ar << ClassHeaderFilename;
        }
    }
    
    if( Ar.LegacyVer >= 655 )
    {
        FName Dummy = NAME_None;
        Ar << Dummy;
    }
    if (Ar.Game == ERedUELegacyGame::Singularity)
    {
        TArray<FName> Unknown2;
        Ar << Unknown2;
        int32 Unknown3;
        Ar << Unknown3;
    }
    Ar << ClassDefaultObject;
    if (ClassDefaultObject)
    {
        URedUELegacySubsystem*RedUELegacySubsystem =  GetTypedOuter<URedUELegacySubsystem>();
        ClassDefaultObject->Rename(nullptr, this);
        RedUELegacySubsystem->ObjectPreload(ClassDefaultObject);
    }
}

void ULegacyProperty::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar << ArrayDim;
    Ar << PropertyFlags;
    if (Ar.Game >= ERedUELegacyGame::UE3)
        Ar << PropertyFlags2;
    Ar << Category;
    if (Ar.Game >= ERedUELegacyGame::UE3)
        Ar << ArraySizeEnum;
    if (PropertyFlags & CPF_Net)
        Ar << RepOffset;
}

void ULegacyByteProperty::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar << Enum;
}

void ULegacyObjectProperty::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar << PropertyClass;
}

void ULegacyClassProperty::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar << MetaClass;
}

void ULegacyArrayProperty::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar << Inner;
}

void ULegacyMapProperty::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
	Ar << Key << Value;
}

void ULegacyStructProperty::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar << Struct;
}

void ULegacyComponentProperty::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
	Ar << SomeName;
}
