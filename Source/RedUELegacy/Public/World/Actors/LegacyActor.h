#pragma once

#include "Core/LegacyObject.h"
#include "LegacyActor.generated.h"

USTRUCT(Blueprintable)
struct FLegacyRotator
{
	GENERATED_BODY()

	bool Serialize(FArchive& Ar);
	
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


UCLASS()
class REDUELEGACY_API ULegacyActor : public ULegacyObject
{
    GENERATED_BODY()
public:
	
	virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;
	
	UPROPERTY(BlueprintReadWrite)
	FVector3f Scale3D = {1,1,1};
    
	UPROPERTY(BlueprintReadWrite)
	FRotator3f Rotation;
    
	UPROPERTY(BlueprintReadWrite)
	FVector3f Translation;
	
	UPROPERTY(BlueprintReadWrite)
	FVector3f XPrivateLocalLocation;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyRotator XPrivateLocalRotation;
	
	UPROPERTY(BlueprintReadWrite)
	FVector3f DrawScale3D = {1,1,1};
   
	
	UPROPERTY(BlueprintReadWrite)
	float DrawScale = 1;
    
    UFUNCTION(BlueprintNativeEvent)
    void Spawn();
};
