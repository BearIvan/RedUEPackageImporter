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
class REDUELEGACY_API ULegacyXMatineeCameraActor : public ULegacyCameraActor
{
	GENERATED_BODY()
public:
	virtual void FillActor_Implementation(AActor* InActor) override;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacySkeletalMeshComponent* SkeletalMeshComponent;
};