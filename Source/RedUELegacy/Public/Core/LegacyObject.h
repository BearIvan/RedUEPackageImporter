﻿#pragma once
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
struct FLegacyMatrix
{
	GENERATED_BODY()

	bool Serialize(FArchive& Ar);
	operator FMatrix44f();
	operator FMatrix();
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
	RLF_NeedLoad = 0x0000020000000000,
};
UCLASS(BlueprintType)
class REDUELEGACY_API ULegacyObject : public UObject
{
	GENERATED_BODY()
public:
    virtual void        LegacyPostLoad              ();
    virtual void        LegacySerialize             (FRedUELegacyArchive& Ar);
	virtual void        LegacySerializeComponent    (FArchive& Ar) { }
    virtual bool        IsAComponent                () { return false; }
    virtual void        PreLegacySerializeUnrealProps	(FRedUELegacyArchive& Ar);
    virtual void        LegacySerializeUnrealProps		(UStruct* Type, void*Object,FRedUELegacyArchive& Ar);
    
    int32           LegacyPackageIndex = INDEX_NONE;
    int64			LegacyObjectFlags = 0;
    int32			NetIndex;

	UFUNCTION(BlueprintCallable,Category="Legacy")
	virtual UObject* ExportToContent();
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FString GetLegacyFullName() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FString GetLegacyName() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FName GetLegacyFName() const;
	
	UFUNCTION(BlueprintCallable,Category="Legacy")
	FString GetOutContentPath() const;
	
    UFUNCTION(BlueprintNativeEvent)
    bool LegacySupport(ERedUELegacyEngineType EngineType,ERedUELegacyGameType GameType);
    
    UFUNCTION(BlueprintNativeEvent)
    FName GetLegacyClassName(ERedUELegacyEngineType EngineType,ERedUELegacyGameType GameType);
    
    UPROPERTY(Transient,BlueprintReadOnly,Category="Legacy")
    ULegacyPackage* LegacyPackage;

	UPROPERTY(Transient,BlueprintReadOnly,Category="Legacy")
	UObject* PresentObject = nullptr;
};
