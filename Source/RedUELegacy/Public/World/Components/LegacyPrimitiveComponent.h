#pragma once

#include "LegacyActorComponent.h"
#include "LegacyPrimitiveComponent.generated.h"

class ULegacyMaterialInterface;
class ULegacyParticleSystem;

UCLASS()
class REDUELEGACY_API ULegacyPrimitiveComponent : public ULegacyActorComponent
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
};

/** Struct used for a particular named instance parameter for this ParticleSystemComponent. */
USTRUCT(Blueprintable)
struct FLegacyParticleSysParam
{
	GENERATED_BODY()

	/** The name of the parameter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ParticleSysParam)
	FName Name;

	/**
	 *	The type of parameters
	 *	PSPT_None       - There is no data type
	 *	PSPT_Scalar     - Use the scalar value
	 *	PSPT_ScalarRand - Select a scalar value in the range [Scalar_Low..Scalar)
	 *	PSPT_Vector     - Use the vector value
	 *	PSPT_VectorRand - Select a vector value in the range [Vector_Low..Vector)
	 *	PSPT_Color      - Use the color value
	 *	PSPT_Actor      - Use the actor value
	 *	PSPT_Material   - Use the material value
	 *	PSPT_VectorUnitRand - Select a random unit vector and scale along the range [Vector_Low..Vector)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ParticleSysParam)
	TEnumAsByte<enum EParticleSysParamType> ParamType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ParticleSysParam)
	float Scalar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ParticleSysParam)
	FVector3f Vector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ParticleSysParam)
	FColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ParticleSysParam)
	class ULegacyActor* Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ParticleSysParam)
	ULegacyMaterialInterface* Material;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleSystemComponent : public ULegacyPrimitiveComponent
{
	GENERATED_BODY()
public:
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyParticleSystem* Template;
	
	UPROPERTY(BlueprintReadWrite)
	bool bSuppressSpawning = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bWasDeactivated = false;
	
	UPROPERTY(BlueprintReadWrite)
	FVector3f PartSysVelocity;

	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyParticleSysParam> InstanceParameters;
	
};