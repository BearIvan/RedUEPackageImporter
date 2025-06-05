#pragma once

#include "Core/LegacyObject.h"
#include "LegacyActor.generated.h"


UCLASS()
class REDUELEGACY_API ULegacyActor : public ULegacyObject
{
    GENERATED_BODY()
public:
	
	virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;

	UFUNCTION(BlueprintNativeEvent)
    UClass* GetActorClass();
	
    UFUNCTION(BlueprintNativeEvent)
    AActor* Spawn();

	UFUNCTION(BlueprintNativeEvent)
	void FillActor( AActor* InActor);
	    
	UPROPERTY(BlueprintReadWrite)
	FLegacyRotator Rotation;
    
	UPROPERTY(BlueprintReadWrite)
	FVector3f Location;
	
	UPROPERTY(BlueprintReadWrite)
	FVector3f XPrivateLocalLocation;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyRotator XPrivateLocalRotation;
	
	UPROPERTY(BlueprintReadWrite)
	FVector3f DrawScale3D = {1,1,1};

	UPROPERTY(BlueprintReadWrite)
	ULegacyActor* Base;
	
	UPROPERTY(BlueprintReadWrite)
	float DrawScale = 1;
};

UCLASS()
class REDUELEGACY_API ULegacyCameraActor : public ULegacyActor
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation() override;
	
};

UCLASS()
class REDUELEGACY_API ULegacyMarker : public ULegacyActor
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation() override;
	
};

UCLASS()
class REDUELEGACY_API ULegacyXMarker : public ULegacyMarker
{
	GENERATED_BODY()
public:
	
};

UCLASS()
class REDUELEGACY_API ULegacyXProxyCameraHeightActor : public ULegacyMarker
{
	GENERATED_BODY()
public:
	
};

UCLASS()
class REDUELEGACY_API ULegacyXMatineeCameraActor : public ULegacyCameraActor
{
	GENERATED_BODY()
public:
	virtual void FillActor_Implementation(AActor* InActor) override;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacySkeletalMeshComponent* SkeletalMeshComponent;
};

UCLASS()
class REDUELEGACY_API ULegacyLevelStreaming: public ULegacyObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FName PackageName;
};

UCLASS()
class REDUELEGACY_API ULegacyLevelStreamingAlwaysLoaded: public ULegacyLevelStreaming
{
	GENERATED_BODY()
public:
};

UCLASS()
class REDUELEGACY_API ULegacyLevelStreamingKismet: public ULegacyLevelStreaming
{
	GENERATED_BODY()
public:
};

UCLASS()
class REDUELEGACY_API ULegacyWorldInfo : public ULegacyActor
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyLevelStreaming*> StreamingLevels;

	virtual	void	GetStreamingLevels(TArray<ULegacyLevelStreaming*> & OutStreamingLevels);
	virtual UClass* GetActorClass_Implementation() override;
	
};


USTRUCT(Blueprintable)
struct FLegacyXWorldInfoStreamingLevelsWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyLevelStreaming*> StreamingLevels;
};

UCLASS()
class REDUELEGACY_API ULegacyXWorldInfo : public ULegacyWorldInfo
{
	GENERATED_BODY()
public:
	virtual	void	GetStreamingLevels(TArray<ULegacyLevelStreaming*> & OutStreamingLevels) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyXWorldInfoStreamingLevelsWrapper StreamingLevelsWrapper;
};

UCLASS()
class REDUELEGACY_API ULegacyXFloatingSection: public ULegacyActor
{
	GENERATED_BODY()
public:
	virtual void	FillActor_Implementation		(AActor* InActor) override;
	virtual UClass* GetActorClass_Implementation	() override;
	
	UPROPERTY(BlueprintReadWrite)
	FGuid FloatingSectionGuid;

	UPROPERTY(BlueprintReadWrite)
	FLegacyMatrix InvInitialTransform;
	

	
};