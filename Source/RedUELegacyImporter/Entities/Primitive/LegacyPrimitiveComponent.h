#pragma once
#include "Entities/LegacyActorComponent.h"
#include "LegacyPrimitiveComponent.generated.h"

class ULegacyMaterialInterface;
class ULegacyParticleSystem;

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyPrimitiveComponent : public ULegacyActorComponent
{
	GENERATED_BODY()

public:
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;

	UPROPERTY(BlueprintReadWrite)
	FVector3f Scale3D = {1,1,1};
    
	UPROPERTY(BlueprintReadWrite)
	FLegacyRotator Rotation;
    
	UPROPERTY(BlueprintReadWrite)
	FVector3f Translation;
	
	UPROPERTY(BlueprintReadWrite)
	bool HiddenGame = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bCastHiddenShadow = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool CollideActors = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool BlockActors = false;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyMeshComponent : public ULegacyPrimitiveComponent
{
	GENERATED_BODY()
};