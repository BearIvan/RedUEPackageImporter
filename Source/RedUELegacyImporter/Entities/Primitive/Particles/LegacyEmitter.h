#pragma once
#include "Entities/LegacyActor.h"
#include "Entities/Primitive/LegacyPrimitiveComponent.h"
#include "LegacyEmitter.generated.h"


USTRUCT(Blueprintable)
struct FLegacyParticleSysParam
{
	GENERATED_BODY()

	/** The name of the parameter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ParticleSysParam)
	FName Name;

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
class REDUELEGACYIMPORTER_API ULegacyParticleSystemComponent : public ULegacyPrimitiveComponent
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

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyEmitter : public ULegacyActor
{
	GENERATED_BODY()
public:
	
	virtual UClass* GetActorClass_Implementation() override;
	virtual void	FillActor_Implementation		(AActor* InActor) override;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyPrimitiveComponent* ParticleSystemComponent;
};