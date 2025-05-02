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
