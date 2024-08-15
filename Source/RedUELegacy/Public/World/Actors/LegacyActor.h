#pragma once

#include "Core/LegacyObject.h"
#include "LegacyActor.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyActor : public ULegacyObject
{
    GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FVector3f Scale3D = {1,1,1};
    
	UPROPERTY(BlueprintReadWrite)
	FRotator3f Rotation;
    
	UPROPERTY(BlueprintReadWrite)
	FVector3f Translation;

	UPROPERTY(BlueprintReadWrite)
	float DrawScale;
    
    UFUNCTION(BlueprintNativeEvent)
    void Spawn();
};
