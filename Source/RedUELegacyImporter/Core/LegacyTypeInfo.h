#pragma once
#include "LegacyObject.h"
#include "LegacyTypeInfo.generated.h"

UCLASS(NotBlueprintable)
class REDUELEGACYIMPORTER_API ULegacyField : public ULegacyObject
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
class REDUELEGACYIMPORTER_API ULegacyEnum : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    TArray<FName>	Names;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyConst : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    FString			Value;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyFunction : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyTextBuffer : public ULegacyObject
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    int32			Pos;
    int32			Top;
    FString			Text;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyStruct : public ULegacyField
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
class REDUELEGACYIMPORTER_API ULegacyScriptStruct : public ULegacyStruct
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyState  : public ULegacyStruct
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
class REDUELEGACYIMPORTER_API ULegacyClass  : public ULegacyState
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyProperty  : public ULegacyField
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
class REDUELEGACYIMPORTER_API ULegacyByteProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyEnum*  Enum;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyIntProperty  : public ULegacyField
{
    GENERATED_BODY()
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyBoolProperty  : public ULegacyField
{
    GENERATED_BODY()
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyFloatProperty  : public ULegacyField
{
    GENERATED_BODY()
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyObjectProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyClass* PropertyClass;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyClassProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyClass* MetaClass;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyNameProperty  : public ULegacyField
{
    GENERATED_BODY()
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyStrProperty  : public ULegacyField
{
    GENERATED_BODY()
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyArrayProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyProperty* Inner;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyMapProperty  : public ULegacyField
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
class REDUELEGACYIMPORTER_API ULegacyStructProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyStruct* Struct;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyComponentProperty  : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyObject* SomeName;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyPointerProperty : public ULegacyProperty
{
    GENERATED_BODY()
};