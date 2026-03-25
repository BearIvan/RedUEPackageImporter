#pragma once
#include "LegacyObject.h"
#include "LegacyTypeInfo.generated.h"

UCLASS(NotBlueprintable)
class REDUELEGACY_API ULegacyField : public ULegacyObject
{
    GENERATED_BODY()

public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyField* SuperField2;
    
    UPROPERTY(Transient)
    ULegacyField* Next;
};

UCLASS()
class REDUELEGACY_API ULegacyEnum : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    TArray<FName>	Names;
};

UCLASS()
class REDUELEGACY_API ULegacyConst : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    FString			Value;
};

UCLASS()
class REDUELEGACY_API ULegacyFunction : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
};


UCLASS()
class REDUELEGACY_API ULegacyTextBuffer : public ULegacyObject
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    int32			Pos;
    int32			Top;
    FString			Text;
};


UCLASS()
class REDUELEGACY_API ULegacyStruct : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    FName			FriendlyName;
    int32			TextPos;
    int32			Line;
    int32			f60;
    TArray<uint8>	Script;
    
    UPROPERTY(Transient)
    ULegacyField* SuperField;

    UPROPERTY(Transient)
    ULegacyTextBuffer* ScriptText;

    UPROPERTY(Transient)
    ULegacyTextBuffer* CppText;

    UPROPERTY(Transient)
    ULegacyField* Children;
};

UCLASS()
class REDUELEGACY_API ULegacyScriptStruct : public ULegacyStruct
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
};

UCLASS()
class REDUELEGACY_API ULegacyState  : public ULegacyStruct
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    int64			ProbeMask;
    int64			IgnoreMask;
    int				StateFlags;
    uint16			LabelTableOffset;
};
UCLASS()
class REDUELEGACY_API ULegacyClass  : public ULegacyState
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
};


UCLASS()
class REDUELEGACY_API ULegacyProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    int32			ArrayDim;
    uint32  		PropertyFlags;
	uint32		    PropertyFlags2;
	FName			Category;
    uint16			RepOffset;
    
    UPROPERTY(Transient)
    ULegacyEnum* ArraySizeEnum;
    
};

UCLASS()
class REDUELEGACY_API ULegacyByteProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyEnum*  Enum;
};


UCLASS()
class REDUELEGACY_API ULegacyIntProperty  : public ULegacyField
{
    GENERATED_BODY()
};


UCLASS()
class REDUELEGACY_API ULegacyBoolProperty  : public ULegacyField
{
    GENERATED_BODY()
};

UCLASS()
class REDUELEGACY_API ULegacyFloatProperty  : public ULegacyField
{
    GENERATED_BODY()
};

UCLASS()
class REDUELEGACY_API ULegacyObjectProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyClass* PropertyClass;
};

UCLASS()
class REDUELEGACY_API ULegacyClassProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyClass* MetaClass;
};

UCLASS()
class REDUELEGACY_API ULegacyNameProperty  : public ULegacyField
{
    GENERATED_BODY()
};

UCLASS()
class REDUELEGACY_API ULegacyStrProperty  : public ULegacyField
{
    GENERATED_BODY()
};

UCLASS()
class REDUELEGACY_API ULegacyArrayProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyProperty* Inner;
};

UCLASS()
class REDUELEGACY_API ULegacyMapProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyProperty* Key;
    
    UPROPERTY(Transient)
    ULegacyProperty* Value;
};

UCLASS()
class REDUELEGACY_API ULegacyStructProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyStruct* Struct;
};


UCLASS()
class REDUELEGACY_API ULegacyComponentProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyObject* SomeName;
};

UCLASS()
class REDUELEGACY_API ULegacyPointerProperty : public ULegacyProperty
{
    GENERATED_BODY()
};