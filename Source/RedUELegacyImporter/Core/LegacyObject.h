#pragma once
#include "LegacyObject.generated.h"


USTRUCT(Blueprintable)
struct FLegacyRotator
{
	GENERATED_BODY()

	bool Serialize(FArchive& Ar);
	operator FRotator3f();
	operator FRotator();
	FLegacyRotator&operator=(const FRotator3f& Rotator);
	INT Pitch; // Looking up and down (0=Straight Ahead, +Up, -Down).
	INT Yaw;   // Rotating around (running in circles), 0=East, +North, -South.
	INT Roll;  // Rotation about axis of screen, 0=Straight, +Clockwise, -CCW.
};



USTRUCT()
struct FLegacyRawDistribution
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (LegacyRead))
	uint8 Type = 0;
	
	UPROPERTY(meta = (LegacyRead))
	uint8 Op = 0;
	
	UPROPERTY(meta = (LegacyRead))
	uint8 LookupTableNumElements = 0;
	
	UPROPERTY(meta = (LegacyRead))
	uint8 LookupTableChunkSize = 0;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<float> LookupTable;
	
	UPROPERTY(meta = (LegacyRead))
	float LookupTableTimeScale = 0;
	
	UPROPERTY(meta = (LegacyRead))
	float LookupTableStartTime = 0;
};


USTRUCT(Blueprintable)
struct FInterpCurveVector3fPoint
{
	GENERATED_BODY()
	
	/** Float input value that corresponds to this key (eg. time). */
	UPROPERTY(BlueprintReadWrite)
	float		InVal;

	/** Output value of templated type when input is equal to InVal. */
	UPROPERTY(BlueprintReadWrite)
	FVector3f	OutVal;

	/** Tangent of curve arrive this point. */
	UPROPERTY(BlueprintReadWrite)
	FVector3f	ArriveTangent; 

	/** Tangent of curve leaving this point. */
	UPROPERTY(BlueprintReadWrite)
	FVector3f	LeaveTangent; 

	/** Interpolation mode between this point and the next one. @see EInterpCurveMode */
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EInterpCurveMode>	InterpMode; 
};

USTRUCT(Blueprintable)
struct FInterpCurveVector3f
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FInterpCurveVector3fPoint> Points;
};

inline FArchive& operator<<(FArchive& Ar, FLegacyRotator& R)
{
	R.Serialize(Ar);
	return Ar;
}

template <>
struct TStructOpsTypeTraits<FLegacyRotator> : public TStructOpsTypeTraitsBase2<FLegacyRotator>
{
	enum
	{
		WithSerializer = true
	};
};


USTRUCT(Blueprintable)
struct REDUELEGACYIMPORTER_API FLegacyMatrix
{
	GENERATED_BODY()

	bool Serialize(FArchive& Ar);
	operator FMatrix44f() const;
	operator FMatrix() const;
	FLegacyMatrix&operator=(const FMatrix44f& InData);
	FMatrix44f Data;
};



inline FArchive& operator<<(FArchive& Ar, FLegacyMatrix& R)
{
	R.Serialize(Ar);
	return Ar;
}

template <>
struct TStructOpsTypeTraits<FLegacyMatrix> : public TStructOpsTypeTraitsBase2<FLegacyMatrix>
{
	enum
	{
		WithSerializer = true
	};
};




class ULegacyPackage;
class FRedUELegacyArchive;

enum ELegacyObjectFlags:int64
{
	RLF_ClassDefaultObject = 0x0000000000000200,
	RLF_ArchetypeObject	   = 0x0000000000000400,
	RLF_NeedLoad		   = 0x0000020000000000,
};
UCLASS(BlueprintType)
class REDUELEGACYIMPORTER_API ULegacyObject : public UObject
{
	GENERATED_BODY()
public:
    virtual void        LegacyPostLoad					();
	virtual void		LegacySerializeDefaultObject	(FRedUELegacyArchive& Ar);
	virtual void        LegacySerialize					(FRedUELegacyArchive& Ar);
	virtual void        LegacySerializeComponent		(FArchive& Ar) ;
    virtual bool        IsAComponent					() { return false; }
    virtual void        PreLegacySerializeUnrealProps	(FRedUELegacyArchive& Ar);
    virtual void        LegacySerializeUnrealProps		(UStruct* Type, void*Object,FRedUELegacyArchive& Ar);
	
			FString		GetPackagePath					(const FString& OutPath) const;
			FString		GetPackagePath					() const;
			FString		GetFullObjectPath				(const FString& OutPath) const;
	
 
	template<typename T,typename M>
	bool TryLoadOrCreate(T*&OutObject,M*&OutNewObject)
    {
    	OutObject = LoadObject<T>(nullptr, *GetFullObjectPath(GetOutContentPath()),nullptr,LOAD_NoWarn);
    	if (OutObject)
    	{
    		return false;
    	}
    	TArray<FString> OptionalContentPaths = GetOptionalContentPaths();
    	for (const FString& OptionalPath:OptionalContentPaths)
    	{
    		if (!OutObject)
    		{
    			OutObject = LoadObject<T>(nullptr, *GetFullObjectPath(OptionalPath),nullptr,LOAD_NoWarn);
    		}
    		else
    		{
    			return false;
    		}
    	}
    	const FString PackageName = GetPackagePath();
    	UPackage*  AssetPackage = CreatePackage(*PackageName);
    	OutNewObject = NewObject<M>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public|RF_Standalone);
    	OutObject = OutNewObject;
		return true;
    }
	
	template<typename T>
	bool TryLoadOrCreate(T*&OutObject)
	{
		return TryLoadOrCreate(OutObject,OutObject);
	}


	int32           LegacyPackageIndex = INDEX_NONE;
    int64			LegacyObjectFlags = 0;
    int32			NetIndex;
	int32			PropertiesOffset = INDEX_NONE;

	UFUNCTION(BlueprintCallable,Category="Legacy")
	virtual UObject* ExportToContent();
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FString GetLegacyFullName() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FString GetLegacyName() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FName GetLegacyFName() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	TArray<FString> GetOptionalContentPaths() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FString GetOutContentPath() const;
	
    UFUNCTION(BlueprintNativeEvent)
    bool LegacySupport(ERedUELegacyEngineType EngineType,ERedUELegacyGameType GameType);
    
    UFUNCTION(BlueprintNativeEvent)
    FName GetLegacyClassName(ERedUELegacyEngineType EngineType,ERedUELegacyGameType GameType);
    
	UFUNCTION(BlueprintCallable,Category="Legacy")
	ULegacyObject* GetLegacyArchetype();
		
	UPROPERTY(Transient,BlueprintReadOnly,Category="Legacy")
    ULegacyPackage* LegacyPackage;

	UPROPERTY(Transient,BlueprintReadOnly,Category="Legacy")
	UObject* PresentObject = nullptr;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyUnknown : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;	
};

UCLASS(EditInlineNew,DefaultToInstanced)
class REDUELEGACYIMPORTER_API ULegacyComponent : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual bool IsAComponent() override { return true; }
        
};