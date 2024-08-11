#include "..\..\Public\Core\LegacyTypeInfo.h"
#include "Core/RedUELegacyArchive.h"

void ULegacyField::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    if (Ar.LegacyVer < 756)
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
    if (Ar.LegacyVer >= 756)
    {
        Ar << SuperField;
    }
    else
    {
        SuperField = SuperField2;
    }
    Ar << Line << TextPos;
    int ScriptSize;
    Ar << ScriptSize;
    if (ScriptSize)
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
    Ar << ProbeMask << IgnoreMask << StateFlags << LabelTableOffset;
}

void ULegacyClass::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar.Seek(Ar.GetStopper());
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
