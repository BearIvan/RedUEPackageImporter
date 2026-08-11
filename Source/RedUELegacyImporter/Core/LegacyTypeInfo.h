#pragma once
#include "LegacyObject.h"
#include "LegacyTypeInfo.generated.h"

class ULegacyClass;

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

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyEnum : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    TArray<FName>	Names;
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyConst : public ULegacyField
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    FString			Value;
};

UCLASS(meta = (LegacyPackage = Core))
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


UCLASS(meta = (LegacyPackage = Core))
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

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyScriptStruct : public ULegacyStruct
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyState  : public ULegacyStruct
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    int64			ProbeMask = 0;
    int64			IgnoreMask = 0;
    int32			StateFlags = 0;
    int16           LabelTableOffset = 0;
    UPROPERTY(Transient)
    TMap<FName,ULegacyFunction*> FuncMap;
};

USTRUCT()
struct FLegacyImplementedInterface
{
    GENERATED_BODY()
    
    UPROPERTY(Transient)
   ULegacyClass* Class = nullptr;
   
    UPROPERTY(Transient)
    ULegacyProperty* PointerProperty = nullptr;
    friend FArchive& operator<<(FArchive& Ar, FLegacyImplementedInterface& R);
};



UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyClass  : public ULegacyState
{
    GENERATED_BODY()
public:
    virtual void        LegacySerialize(FRedUELegacyArchive& Ar) override;
    uint32			    ClassFlags;
    uint8			    ClassPlatformFlags;
    FName			    ClassConfigName;
    TArray<FName>		HideCategories;
    TArray<FName>		AutoExpandCategories;
    TArray<FName>		AutoCollapseCategories;
    TArray<FName>		DontSortCategories;
    TArray<FName>       DependentOn;
    TArray<FName>		ClassGroupNames;
    bool				bForceScriptOrder;
    FString				ClassHeaderFilename;
    
    UPROPERTY(Transient)
    UClass*	ClassWithin;
    
    UPROPERTY(Transient)
    TMap<FName,class ULegacyObject*>	ComponentNameToDefaultObjectMap;
    
    UPROPERTY(Transient)
    TArray<FLegacyImplementedInterface> Interfaces;
    
    UPROPERTY(Transient)
    ULegacyObject*	ClassDefaultObject;
};


UCLASS(meta = (LegacyPackage = Core))
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

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyByteProperty  : public ULegacyProperty
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyEnum*  Enum;
};


UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyIntProperty  : public ULegacyProperty
{
    GENERATED_BODY()
};


UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyBoolProperty  : public ULegacyProperty
{
    GENERATED_BODY()
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyFloatProperty  : public ULegacyProperty
{
    GENERATED_BODY()
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyObjectProperty  : public ULegacyProperty
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyClass* PropertyClass;
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyClassProperty  : public ULegacyProperty
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyClass* MetaClass;
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyNameProperty  : public ULegacyProperty
{
    GENERATED_BODY()
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyStrProperty  : public ULegacyProperty
{
    GENERATED_BODY()
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyArrayProperty  : public ULegacyProperty
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyProperty* Inner;
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyMapProperty  : public ULegacyProperty
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyProperty* Key;
    
    UPROPERTY(Transient)
    ULegacyProperty* Value;
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyStructProperty  : public ULegacyProperty
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyStruct* Struct;
};


UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyComponentProperty  : public ULegacyProperty
{
    GENERATED_BODY()
public:
    virtual void    LegacySerialize(FRedUELegacyArchive& Ar) override;
    
    UPROPERTY(Transient)
    ULegacyObject* SomeName;
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyPointerProperty : public ULegacyProperty
{
    GENERATED_BODY()
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyInterface : public ULegacyObject
{
    GENERATED_BODY()
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacySubsystem : public ULegacyObject
{
    GENERATED_BODY()
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyDelegateProperty : public ULegacyProperty
{
    GENERATED_BODY()
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyQwordProperty : public ULegacyProperty
{
    GENERATED_BODY()
};

UCLASS(meta = (LegacyPackage = Core))
class REDUELEGACYIMPORTER_API ULegacyInterfaceProperty : public ULegacyProperty
{
    GENERATED_BODY()
};
