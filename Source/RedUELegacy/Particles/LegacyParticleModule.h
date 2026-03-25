#pragma once

#include "Core/LegacyObject.h"
#include "Particles/ParticleSpriteEmitter.h"
#include "Particles/Orbit/ParticleModuleOrbit.h"
#include "Particles/Parameter/ParticleModuleParameterDynamic.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "LegacyParticleModule.generated.h"


USTRUCT()
struct FLegacyRawDistribution
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (LegacyRead))
	uint8 Type = 0;
	
	UPROPERTY(meta = (LegacyRead))
	uint8 Op = 0;
	
	UPROPERTY(meta = (LegacyRead))
	uint8 LookupTableNumElements = 0;
	
	UPROPERTY(meta = (LegacyRead))
	uint8 LookupTableChunkSize = 0;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<float> LookupTable;
	
	UPROPERTY(meta = (LegacyRead))
	float LookupTableTimeScale = 0;
	
	UPROPERTY(meta = (LegacyRead))
	float LookupTableStartTime = 0;
};

USTRUCT()
struct FLegacyRawDistributionFloat:public FLegacyRawDistribution
{
	GENERATED_BODY()
public:
	UDistributionFloat* MakeDistributionFloat(UObject*Outer) const;
};

USTRUCT()
struct FLegacyRawDistributionVector:public FLegacyRawDistribution
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (LegacyRead))
	ULegacyObject* Distribution;
	
	UDistributionVector* MakeDistributionVector(UObject*Outer) const;
};

UCLASS()
class REDUELEGACY_API ULegacyDistributionVectorParticleParameter : public ULegacyObject
{
	GENERATED_BODY()
public:
	
	virtual bool IsAComponent() override;
	virtual UDistributionVectorParameterBase* Export(UObject* Outer);

	UPROPERTY(BlueprintReadWrite)
	FName ParameterName;

	UPROPERTY(BlueprintReadWrite)
	FVector3f MinInput;

	UPROPERTY(BlueprintReadWrite)
	FVector3f MaxInput;

	UPROPERTY(BlueprintReadWrite)
	FVector3f MinOutput;

	UPROPERTY(BlueprintReadWrite)
	FVector3f MaxOutput;

	UPROPERTY(meta = (LegacyRead))
	TEnumAsByte<DistributionParamMode> ParamModes[3];

	UPROPERTY(BlueprintReadWrite)
	FVector3f Constant;
	
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModule : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem);
	
	UPROPERTY(BlueprintReadWrite)
	uint8 LODValidity;

	UPROPERTY(BlueprintReadWrite)
	bool bEnabled = true;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleRequired: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
		
	UPROPERTY(BlueprintReadWrite)
	ULegacyObject* Material;
	
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EParticleScreenAlignment> ScreenAlignment;

	UPROPERTY(BlueprintReadWrite)
	bool bUseLocalSpace= false;

	UPROPERTY(BlueprintReadWrite)
	bool bUseLegacyEmitterTime = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bUseMaxDrawCount = true;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxDrawCount = 500;

	UPROPERTY(BlueprintReadWrite)
	float RandomImageTime = 0;

	UPROPERTY(BlueprintReadWrite)
	float EmitterDuration = 1;
	
	UPROPERTY(BlueprintReadWrite)
	int32 EmitterLoops = 0;
};

USTRUCT()
struct FLegacyParticleBurst
{
	GENERATED_BODY()

	/** The number of particles to burst */
	UPROPERTY(meta = (LegacyRead))
	int32 Count;

	/** If >= 0, use as a range [CountLow..Count] */
	UPROPERTY(meta = (LegacyRead))
	int32 CountLow;

	/** The time at which to burst them (0..1: emitter lifetime) */
	UPROPERTY(meta = (LegacyRead))
	float Time;



	FLegacyParticleBurst()
	: Count(0)
	, CountLow(-1)		// Disabled by default...
	, Time(0.0f)
	{
	}
	
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleSpawn: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat Rate;

	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat RateScale;
	
	UPROPERTY(meta = (LegacyRead))
	TEnumAsByte<EParticleBurstMethod>	ParticleBurstMethod;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<FLegacyParticleBurst>	BurstList;
	
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleLifetime: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat Lifetime;
	
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleSize: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector StartSize;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleVelocityBase: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (LegacyRead))
	bool bInWorldSpace;
	
	UPROPERTY(meta = (LegacyRead))
	bool bApplyOwnerScale;
};


UCLASS()
class REDUELEGACY_API ULegacyParticleModuleVelocity: public ULegacyParticleModuleVelocityBase
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector StartVelocity;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat StartVelocityRadial;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleLocation: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector StartLocation;

	UPROPERTY(meta = (LegacyRead))
	float DistributeOverNPoints;

	UPROPERTY(meta = (LegacyRead))
	float DistributeThreshold;

};


UCLASS()
class REDUELEGACY_API ULegacyParticleModuleSizeMultiplyLife: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector LifeMultiplier;

	UPROPERTY(meta = (LegacyRead))
	bool MultiplyX = true;
	
	UPROPERTY(meta = (LegacyRead))
	bool MultiplyY = true;
	
	UPROPERTY(meta = (LegacyRead))
	bool MultiplyZ = true;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleRotation: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat StartRotation;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleRotationRate: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat StartRotationRate;
};


UCLASS()
class REDUELEGACY_API ULegacyParticleModuleColor: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector StartColor;

	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat StartAlpha;

	UPROPERTY(meta = (LegacyRead))
	bool bClampAlpha;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleColorScaleOverLife: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector ColorScaleOverLife;

	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat AlphaScaleOverLife;
	
	UPROPERTY(meta = (LegacyRead))
	bool bEmitterTime;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleAcceleration: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector Acceleration;
	
	UPROPERTY(meta = (LegacyRead))
	bool bApplyOwnerScale;

	UPROPERTY(meta = (LegacyRead))
	bool bAlwaysInWorldSpace;
};

USTRUCT()
struct FLegacyEmitterDynamicParameter
{
	GENERATED_BODY()
	/** The parameter name - from the material DynamicParameter expression. READ-ONLY */
	UPROPERTY(meta = (LegacyRead))
	FName ParamName;
	
	/** If TRUE, use the EmitterTime to retrieve the value, otherwise use Particle RelativeTime. */
	UPROPERTY(meta = (LegacyRead))
	uint8 bUseEmitterTime : 1;
	
	/** If TRUE, only set the value at spawn time of the particle, otherwise update each frame. */
	UPROPERTY(meta = (LegacyRead))
	uint8 bSpawnTimeOnly : 1;
	
	/** Where to get the parameter value from. */
	UPROPERTY(meta = (LegacyRead))
	TEnumAsByte<enum EEmitterDynamicParameterValue>  ValueMethod;
	
	/** If TRUE, scale the velocity value selected in ValueMethod by the evaluated ParamValue. */
	UPROPERTY(meta = (LegacyRead))
	uint8 bScaleVelocityByParamValue : 1;
	
	/** The distriubtion for the parameter value. */
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat ParamValue;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleParameterDynamic: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<FLegacyEmitterDynamicParameter> DynamicParams;

	UPROPERTY(meta = (LegacyRead))
	bool bUsesVelocity = true;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleAttractorPoint: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector Position;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat Range;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat Strength;
	
	UPROPERTY(meta = (LegacyRead))
	bool StrengthByDistance;

	UPROPERTY(meta = (LegacyRead))
	bool bAffectBaseVelocity;

	UPROPERTY(meta = (LegacyRead))
	bool bOverrideVelocity;

	UPROPERTY(meta = (LegacyRead))
	bool bUseWorldSpacePosition;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleColorOverLife: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector ColorOverLife;

	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionFloat AlphaOverLife;
	
	UPROPERTY(meta = (LegacyRead))
	bool bClampAlpha;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleTypeDataMesh: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;
	
	UPROPERTY(meta = (LegacyRead))
	ULegacyObject* Mesh;
	
	UPROPERTY(meta = (LegacyRead))
	TEnumAsByte<EMeshScreenAlignment> MeshAlignment = PSMA_MeshFaceCameraWithRoll;
	
	UPROPERTY(meta = (LegacyRead))
	bool bOverrideMaterial;
	
	UPROPERTY(meta = (LegacyRead))
	float Pitch;
	
	UPROPERTY(meta = (LegacyRead))
	float Roll;
	
	UPROPERTY(meta = (LegacyRead))
	float Yaw;
	
	UPROPERTY(meta = (LegacyRead))
	TEnumAsByte<EParticleAxisLock> AxisLockOption = EPAL_NONE;

	UPROPERTY(meta = (LegacyRead))
	bool	bCameraFacing;
	
	UPROPERTY(meta = (LegacyRead))
	TEnumAsByte<EMeshCameraFacingOptions> CameraFacingOption = XAxisFacing_NoUp;
	
	UPROPERTY(meta = (LegacyRead))
	bool bApplyParticleRotationAsSpin;

};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleMeshRotation: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;

	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector StartRotation;
	
	UPROPERTY(meta = (LegacyRead))
	bool bInheritParent;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleOrbit: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;

	UPROPERTY(meta = (LegacyRead))
	TEnumAsByte<EOrbitChainMode> ChainMode = EOChainMode_Link;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector OffsetAmount;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector RotationAmount;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector RotationRateAmount;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleLocationDirect: public ULegacyParticleModule
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;

	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector Location;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector LocationOffset;

	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector ScaleFactor;
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector Direction;
};

UCLASS()
class REDUELEGACY_API ULegacyParticleModuleVelocityOverLifetime: public ULegacyParticleModuleVelocityBase
{
	GENERATED_BODY()
public:
	virtual UParticleModule* Export(UParticleSystem* ParticleSystem) override;

	UPROPERTY(meta = (LegacyRead))
	FLegacyRawDistributionVector VelOverLife;
	
	UPROPERTY(meta = (LegacyRead))
	bool Absolute = true;
};
