#pragma once

#include "Core/LegacyObject.h"
#include "LegacyParticleSystem.generated.h"


UCLASS()
class REDUELEGACY_API ULegacyParticleLODLevel : public ULegacyObject
{
	GENERATED_BODY()
public:
	void Export(UParticleSystem* ParticleSystem, const TObjectPtr<UParticleLODLevel>& ParticleLODLevel);

	UPROPERTY(BlueprintReadWrite)
	ULegacyParticleModuleRequired* RequiredModule = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyParticleModule*> Modules;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyParticleModule* SpawnModule = nullptr;

	UPROPERTY(BlueprintReadWrite)
	ULegacyParticleModule* TypeDataModule = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	int32 PeakActiveParticles = 0;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleSpriteEmitter : public ULegacyObject
{
	GENERATED_BODY()
public:
	void Export(UParticleSystem* NewParticleSystem);
	
	UPROPERTY(BlueprintReadWrite)
	FName EmitterName;

	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyParticleLODLevel*> LODLevels;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleSystem : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual UObject*	ExportToContent						() override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyParticleSpriteEmitter*> Emitters;

	UPROPERTY(BlueprintReadWrite)
	TArray<float> LODDistances;
};
