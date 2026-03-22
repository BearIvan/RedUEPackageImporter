#include "Particles/LegacyParticleModule.h"

#include "Particles/ParticleModuleRequired.h"
#include "Particles/Acceleration/ParticleModuleAcceleration.h"
#include "Particles/Attractor/ParticleModuleAttractorPoint.h"
#include "Particles/Color/ParticleModuleColor.h"
#include "Particles/Color/ParticleModuleColorOverLife.h"
#include "Particles/Color/ParticleModuleColorScaleOverLife.h"
#include "Particles/Lifetime/ParticleModuleLifetime.h"
#include "Particles/Location/ParticleModuleLocation.h"
#include "Particles/Location/ParticleModuleLocationDirect.h"
#include "Particles/Parameter/ParticleModuleParameterDynamic.h"
#include "Particles/Rotation/ParticleModuleMeshRotation.h"
#include "Particles/Rotation/ParticleModuleRotation.h"
#include "Particles/RotationRate/ParticleModuleRotationRate.h"
#include "Particles/Size/ParticleModuleSize.h"
#include "Particles/Size/ParticleModuleSizeMultiplyLife.h"
#include "Particles/Spawn/ParticleModuleSpawn.h"
#include "Particles/Velocity/ParticleModuleVelocity.h"
#include "Particles/Velocity/ParticleModuleVelocityOverLifetime.h"

UDistributionFloat* FLegacyRawDistributionFloat::MakeDistributionFloat(UObject* Outer) const
{ 
	if (LookupTable.Num() == 0)
	{
		return nullptr;
	}
	if (Op == 0 || Op == 1)
	{
		if (FMath::IsNearlyZero(LookupTableTimeScale))
		{
			UDistributionFloatConstant* Result =  NewObject<UDistributionFloatConstant>(Outer);
			Result->Constant = LookupTable[2];
			return Result;	
		}
		else
		{
			UDistributionFloatConstantCurve* Result =  NewObject<UDistributionFloatConstantCurve>(Outer);
			for (int32 i = 0;i<LookupTable.Num() - 2;i++)
			{
				Result->ConstantCurve.AddPoint(i/LookupTableTimeScale + LookupTableStartTime, LookupTable[i + 2]);
			}
			return Result;
		}
	}
	else if (Op == 2||Op == 3)
	{
		if (FMath::IsNearlyZero(LookupTableTimeScale))
		{
			UDistributionFloatUniform* Result =  NewObject<UDistributionFloatUniform>(Outer);
			Result->Min = LookupTable[2];
			Result->Max = LookupTable[3];
			return Result;
		}
		else
		{
			ensure(false);
		}
	}
	else
	{
		ensureMsgf(false,TEXT("Unknown op:%d"),Op);
	}
	return 	nullptr;
}

UDistributionVector* FLegacyRawDistributionVector::MakeDistributionVector(UObject* Outer) const
{
	if (ULegacyDistributionVectorParticleParameter* DistributionParameter = Cast<ULegacyDistributionVectorParticleParameter>(Distribution))
	{
		return DistributionParameter->Export(Outer);
	}
	if (LookupTable.Num() == 0)
	{
		return nullptr;
	}
	if (Op == 0 || Op == 1)
	{
		if (FMath::IsNearlyZero(LookupTableTimeScale))
		{
			UDistributionVectorConstant* Result =  NewObject<UDistributionVectorConstant>(Outer);
			Result->Constant.X = LookupTable[2];
			Result->Constant.Y = LookupTable[3];
			Result->Constant.Z = LookupTable[4];
			return Result;	
		}
		else
		{
			UDistributionVectorConstantCurve* Result =  NewObject<UDistributionVectorConstantCurve>(Outer);
			for (int32 i = 0;i<(LookupTable.Num() - 2)/3;i++)
			{
				FVector InVector = { LookupTable[i*3 + 2],LookupTable[i*3 + 3],LookupTable[i*3 + 4]};
				Result->ConstantCurve.AddPoint(i/LookupTableTimeScale + LookupTableStartTime, InVector);
			}
			return Result;
		}
	}
	else if (Op == 2||Op == 3)
	{
		if (FMath::IsNearlyZero(LookupTableTimeScale))
		{
			ensure((Type&~0x7) == 0);
			UDistributionVectorUniform* Result =  NewObject<UDistributionVectorUniform>(Outer);
			Result->Min.X = LookupTable[2];
			Result->Min.Y = LookupTable[3];
			Result->Min.Z = LookupTable[4];
			
			Result->Max.X = LookupTable[5];
			Result->Max.Y = LookupTable[6];
			Result->Max.Z = LookupTable[7];
			Result->bLockAxes = (Type&0x7) == 0;
			Result->LockedAxes = TEnumAsByte<enum EDistributionVectorLockFlags> (Type&0x7);
			return Result;
		}
		else
		{
			ensure(false);
		}
	}
	else
	{
		ensureMsgf(false,TEXT("Unknown op:%d"),Op);
	}
	return 	nullptr;
}

bool ULegacyDistributionVectorParticleParameter::IsAComponent()
{
	return true;
}

UDistributionVectorParameterBase* ULegacyDistributionVectorParticleParameter::Export(UObject* Outer)
{
	UDistributionVectorParameterBase* Result = NewObject<UDistributionVectorParticleParameter>(Outer, NAME_None);
	Result->ParamModes[0] = ParamModes[0];
	Result->ParamModes[1] = ParamModes[1];
	Result->ParamModes[2] = ParamModes[2];
	Result->MaxInput = FVector(MaxInput);
	Result->MinInput = FVector(MinInput);
	Result->MaxOutput = FVector(MaxOutput);
	Result->MaxInput = FVector(MaxInput);
	Result->Constant = FVector(Constant);
	Result->ParameterName = ParameterName;
	return Result;
}

UParticleModule* ULegacyParticleModule::Export(UParticleSystem* ParticleSystem)
{
	checkNoEntry();
	return nullptr;
}

UParticleModule* ULegacyParticleModuleRequired::Export(UParticleSystem* ParticleSystem)
{
	
	UParticleModuleRequired* ParticleModuleRequired = NewObject<UParticleModuleRequired>(ParticleSystem,NAME_None);
	if (Material)
	{
		ParticleModuleRequired->Material = Cast<UMaterialInterface>(Material->ExportToContent());
	}
	ParticleModuleRequired->ScreenAlignment = ScreenAlignment;
	ParticleModuleRequired->bUseLegacyEmitterTime = bUseLegacyEmitterTime;
	ParticleModuleRequired->bUseMaxDrawCount = bUseMaxDrawCount;
	ParticleModuleRequired->MaxDrawCount = MaxDrawCount;
	ParticleModuleRequired->RandomImageTime = RandomImageTime;
	ParticleModuleRequired->LODValidity = LODValidity;
	ParticleModuleRequired->EmitterDuration = EmitterDuration;
	ParticleModuleRequired->EmitterLoops = EmitterLoops;
	ParticleModuleRequired->bUseLocalSpace = bUseLocalSpace;
	return ParticleModuleRequired;
}

UParticleModule* ULegacyParticleModuleSpawn::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleSpawn* ParticleModuleSpawn = NewObject<UParticleModuleSpawn>(ParticleSystem,NAME_None);
	ParticleModuleSpawn->LODValidity = LODValidity;
	ParticleModuleSpawn->bEnabled = bEnabled;
	ParticleModuleSpawn->Rate.Distribution = Rate.MakeDistributionFloat(ParticleModuleSpawn);
	if (UDistributionFloat* NewRateScale = RateScale.MakeDistributionFloat(ParticleModuleSpawn))
	{
		ParticleModuleSpawn->RateScale.Distribution = NewRateScale;
	}
	ParticleModuleSpawn->BurstList.Empty();
	for (const FLegacyParticleBurst& Burst : BurstList)
	{
		FParticleBurst& NewBurst = ParticleModuleSpawn->BurstList.AddDefaulted_GetRef();
		NewBurst.Time = Burst.Time;
		NewBurst.Count = Burst.Count;
		NewBurst.CountLow = Burst.CountLow;
	}
	ParticleModuleSpawn->ParticleBurstMethod = ParticleBurstMethod;
	return ParticleModuleSpawn;
}

UParticleModule* ULegacyParticleModuleLifetime::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleLifetime* ModuleLifetime = NewObject<UParticleModuleLifetime>(ParticleSystem,NAME_None);
	ModuleLifetime->LODValidity = LODValidity;
	ModuleLifetime->bEnabled = bEnabled;
	ModuleLifetime->Lifetime.Distribution = Lifetime.MakeDistributionFloat(ModuleLifetime);
	return ModuleLifetime;
}

UParticleModule* ULegacyParticleModuleSize::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleSize* ModuleLifetime = NewObject<UParticleModuleSize>(ParticleSystem,NAME_None);
	ModuleLifetime->LODValidity = LODValidity;
	ModuleLifetime->bEnabled = bEnabled;
	ModuleLifetime->StartSize.Distribution = StartSize.MakeDistributionVector(ModuleLifetime);
	return ModuleLifetime;
}

UParticleModule* ULegacyParticleModuleVelocity::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleVelocity* ModuleVelocity = NewObject<UParticleModuleVelocity>(ParticleSystem,NAME_None);
	ModuleVelocity->LODValidity = LODValidity;
	ModuleVelocity->bEnabled = bEnabled;
	ModuleVelocity->StartVelocity.Distribution = StartVelocity.MakeDistributionVector(ModuleVelocity);
	ModuleVelocity->StartVelocityRadial.Distribution = StartVelocityRadial.MakeDistributionFloat(ModuleVelocity);
	ModuleVelocity->bApplyOwnerScale = bApplyOwnerScale;
	ModuleVelocity->bInWorldSpace = bInWorldSpace;
	return ModuleVelocity;
}

UParticleModule* ULegacyParticleModuleLocation::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleLocation* ModuleLocation = NewObject<UParticleModuleLocation>(ParticleSystem,NAME_None);
	ModuleLocation->LODValidity = LODValidity;
	ModuleLocation->bEnabled = bEnabled;
	ModuleLocation->StartLocation.Distribution = StartLocation.MakeDistributionVector(ModuleLocation);
	ModuleLocation->DistributeOverNPoints = DistributeOverNPoints;
	ModuleLocation->DistributeThreshold = DistributeThreshold;
	return ModuleLocation;
}

UParticleModule* ULegacyParticleModuleSizeMultiplyLife::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleSizeMultiplyLife* Module = NewObject<UParticleModuleSizeMultiplyLife>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->LifeMultiplier.Distribution = LifeMultiplier.MakeDistributionVector(Module);
	Module->MultiplyX = MultiplyX;
	Module->MultiplyY = MultiplyY;
	Module->MultiplyZ = MultiplyZ;
	return Module;
}

UParticleModule* ULegacyParticleModuleRotation::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleRotation* Module = NewObject<UParticleModuleRotation>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->StartRotation.Distribution = StartRotation.MakeDistributionFloat(Module);
	return Module;
}

UParticleModule* ULegacyParticleModuleRotationRate::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleRotationRate* Module = NewObject<UParticleModuleRotationRate>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->StartRotationRate.Distribution = StartRotationRate.MakeDistributionFloat(Module);
	return Module;
}

UParticleModule* ULegacyParticleModuleColor::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleColor* Module = NewObject<UParticleModuleColor>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->StartColor.Distribution = StartColor.MakeDistributionVector(Module);
	Module->StartAlpha.Distribution = StartAlpha.MakeDistributionFloat(Module);
	Module->bClampAlpha = bClampAlpha;
	return Module;
}

UParticleModule* ULegacyParticleModuleColorScaleOverLife::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleColorScaleOverLife* Module = NewObject<UParticleModuleColorScaleOverLife>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->ColorScaleOverLife.Distribution = ColorScaleOverLife.MakeDistributionVector(Module);
	Module->AlphaScaleOverLife.Distribution = AlphaScaleOverLife.MakeDistributionFloat(Module);
	Module->bEmitterTime = bEmitterTime;
	return Module;
}

UParticleModule* ULegacyParticleModuleAcceleration::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleAcceleration* Module = NewObject<UParticleModuleAcceleration>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->Acceleration.Distribution = Acceleration.MakeDistributionVector(Module);
	Module->bApplyOwnerScale = bApplyOwnerScale;
	Module->bAlwaysInWorldSpace = bAlwaysInWorldSpace;
	return Module;
}

UParticleModule* ULegacyParticleModuleParameterDynamic::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleParameterDynamic* Module = NewObject<UParticleModuleParameterDynamic>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->DynamicParams.Empty();
	for (const FLegacyEmitterDynamicParameter& DynamicParam : DynamicParams)
	{
		FEmitterDynamicParameter&NewEmitterDynamicParameter = Module->DynamicParams.AddDefaulted_GetRef();
		NewEmitterDynamicParameter.ParamName = DynamicParam.ParamName;
		NewEmitterDynamicParameter.bSpawnTimeOnly = DynamicParam.bSpawnTimeOnly;
		NewEmitterDynamicParameter.bScaleVelocityByParamValue = DynamicParam.bScaleVelocityByParamValue;
		NewEmitterDynamicParameter.bUseEmitterTime = DynamicParam.bUseEmitterTime;
		NewEmitterDynamicParameter.ParamValue.Distribution = DynamicParam.ParamValue.MakeDistributionFloat(Module);
		NewEmitterDynamicParameter.ValueMethod = DynamicParam.ValueMethod;
	}
	
	Module->bUsesVelocity = bUsesVelocity;
	return Module;
}

UParticleModule* ULegacyParticleModuleAttractorPoint::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleAttractorPoint* Module = NewObject<UParticleModuleAttractorPoint>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->Position.Distribution = Position.MakeDistributionVector(Module);
	Module->Range.Distribution = Range.MakeDistributionFloat(Module);
	Module->Strength.Distribution = Strength.MakeDistributionFloat(Module);
	Module->bAffectBaseVelocity = bAffectBaseVelocity;
	Module->bOverrideVelocity = bOverrideVelocity;
	Module->bUseWorldSpacePosition = bUseWorldSpacePosition;
	Module->StrengthByDistance = StrengthByDistance;
	return Module;
	
}

UParticleModule* ULegacyParticleModuleColorOverLife::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleColorOverLife* Module = NewObject<UParticleModuleColorOverLife>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->ColorOverLife.Distribution = ColorOverLife.MakeDistributionVector(Module);
	Module->AlphaOverLife.Distribution = AlphaOverLife.MakeDistributionFloat(Module);
	Module->bClampAlpha = bClampAlpha;
	return Module;
}

UParticleModule* ULegacyParticleModuleTypeDataMesh::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleTypeDataMesh* Module = NewObject<UParticleModuleTypeDataMesh>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	if (Mesh)
	{
		Module->Mesh = Cast<UStaticMesh>(Mesh->ExportToContent());
	}
	Module->bOverrideMaterial = bOverrideMaterial;
	Module->bCameraFacing = bCameraFacing;
	Module->MeshAlignment = MeshAlignment;

	if (UDistributionVectorConstant* DistributionVectorConstant= Cast<UDistributionVectorConstant>( Module->RollPitchYawRange.Distribution))
	{
		DistributionVectorConstant->Constant.Set(Roll,Pitch,Yaw);
	}
	
	Module->AxisLockOption = AxisLockOption;
	Module->bCameraFacing = bCameraFacing;
	Module->CameraFacingOption = CameraFacingOption;
	Module->bApplyParticleRotationAsSpin = bApplyParticleRotationAsSpin;
	return Module;
	
}

UParticleModule* ULegacyParticleModuleMeshRotation::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleMeshRotation* Module = NewObject<UParticleModuleMeshRotation>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->StartRotation.Distribution = StartRotation.MakeDistributionVector(Module);
	Module->bInheritParent = bInheritParent;
	return Module;
}

UParticleModule* ULegacyParticleModuleOrbit::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleOrbit* Module = NewObject<UParticleModuleOrbit>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->ChainMode = ChainMode;
	Module->OffsetAmount.Distribution = OffsetAmount.MakeDistributionVector(Module);
	Module->RotationAmount.Distribution = RotationAmount.MakeDistributionVector(Module);
	Module->RotationRateAmount.Distribution = RotationRateAmount.MakeDistributionVector(Module);
	return Module;
}

UParticleModule* ULegacyParticleModuleLocationDirect::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleLocationDirect* Module = NewObject<UParticleModuleLocationDirect>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->Location.Distribution = Location.MakeDistributionVector(Module);
	Module->LocationOffset.Distribution = LocationOffset.MakeDistributionVector(Module);
	Module->Direction.Distribution = Direction.MakeDistributionVector(Module);
	Module->ScaleFactor.Distribution = ScaleFactor.MakeDistributionVector(Module);
	return Module;
}

UParticleModule* ULegacyParticleModuleVelocityOverLifetime::Export(UParticleSystem* ParticleSystem)
{
	UParticleModuleVelocityOverLifetime* Module = NewObject<UParticleModuleVelocityOverLifetime>(ParticleSystem,NAME_None);
	Module->LODValidity = LODValidity;
	Module->bEnabled = bEnabled;
	Module->Absolute = Absolute;
	Module->VelOverLife.Distribution = VelOverLife.MakeDistributionVector(Module);
	return Module;
}
