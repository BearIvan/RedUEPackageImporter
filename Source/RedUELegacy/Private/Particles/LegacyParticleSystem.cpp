#include "Particles/LegacyParticleSystem.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Particles/LegacyParticleModule.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/ParticleSpriteEmitter.h"
#include "Particles/Spawn/ParticleModuleSpawn.h"

void ULegacyParticleLODLevel::Export(UParticleSystem* ParticleSystem, const TObjectPtr<UParticleLODLevel>& LODLevel, int32 LOD)
{
	if (LOD>0)
	{
		return;
	}
	if (RequiredModule)
	{
		LODLevel->RequiredModule = CastChecked<UParticleModuleRequired>(RequiredModule->Export(ParticleSystem));
	}
	if (SpawnModule)
	{
		LODLevel->SpawnModule = CastChecked<UParticleModuleSpawn>(SpawnModule->Export(ParticleSystem));
	}
	if (TypeDataModule)
	{
		LODLevel->TypeDataModule = CastChecked<UParticleModuleTypeDataBase>(TypeDataModule->Export(ParticleSystem));
	}

	for (ULegacyParticleModule* Module : Modules)
	{
		if (Module)
		{
			LODLevel->Modules.Add(Module->Export(ParticleSystem));
		}
	}
	LODLevel->PeakActiveParticles = PeakActiveParticles;
}

void ULegacyParticleSpriteEmitter::Export(UParticleSystem* NewParticleSystem)
{
	UParticleEmitter* NewParticleSpriteEmitter = NewObject<UParticleSpriteEmitter>(NewParticleSystem,NAME_None,RF_Public|RF_Transactional);
	NewParticleSpriteEmitter->PreEditChange(nullptr);
	NewParticleSpriteEmitter->SetEmitterName(EmitterName);
	int32 LodIndex = 0;
	for (ULegacyParticleLODLevel*LegacyParticleLOD:LODLevels)
	{
		int32 Index = NewParticleSpriteEmitter->CreateLODLevel(LodIndex++);
		LegacyParticleLOD->Export(NewParticleSystem, NewParticleSpriteEmitter->LODLevels[Index],Index);
	}

	NewParticleSpriteEmitter->UpdateModuleLists();
	NewParticleSpriteEmitter->PostEditChange();
	if (LODLevels.IsEmpty())
	{
		NewParticleSpriteEmitter->CreateLODLevel(LodIndex);
	}
	else
	{
		for (int32 LODIndex = 0; LODIndex < NewParticleSpriteEmitter->LODLevels.Num(); LODIndex++)
		{
			if (UParticleLODLevel* NewEmitterLODLevel = NewParticleSpriteEmitter->GetLODLevel(LODIndex))
			{
				NewEmitterLODLevel->SetFlags(RF_Transactional);
				check(NewEmitterLODLevel->RequiredModule);
				NewEmitterLODLevel->RequiredModule->SetTransactionFlag();
				check(NewEmitterLODLevel->SpawnModule);
				NewEmitterLODLevel->SpawnModule->SetTransactionFlag();
				for (int32 jj = 0; jj < NewEmitterLODLevel->Modules.Num(); jj++)
				{
					UParticleModule* pkModule = NewEmitterLODLevel->Modules[jj];
					pkModule->SetTransactionFlag();
				}
			}
		}
	}
	NewParticleSystem->Emitters.Add(NewParticleSpriteEmitter);
}

UObject* ULegacyParticleSystem::ExportToContent()
{
	if(PresentObject)
	{
		return PresentObject;
	}
	const FString ObjectPath = GetOutContentPath()/ GetLegacyFullName().Replace(TEXT("."),TEXT("/"));
	const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
	const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
	UParticleSystem* Result = LoadObject<UParticleSystem>(nullptr, *FullObjectPath,nullptr,LOAD_NoWarn);
	if(!Result)
	{
		UPackage*  AssetPackage = CreatePackage(*PackageName);
		UParticleSystem* NewParticleSystem = NewObject<UParticleSystem>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public|RF_Standalone);
		NewParticleSystem->PreEditChange(nullptr);

		NewParticleSystem->LODSettings.AddUninitialized(LODDistances.Num());
		for (int32 LODIndex = 0; LODIndex < NewParticleSystem->LODSettings.Num(); LODIndex++)
		{
			NewParticleSystem->LODSettings[LODIndex] = FParticleSystemLOD::CreateParticleSystemLOD();
			NewParticleSystem->LODDistances.Add(LODDistances[LODIndex]);
		}
		
		for (ULegacyParticleSpriteEmitter* LegacyEmitter : Emitters)
		{
			LegacyEmitter->Export(NewParticleSystem);
		}
		NewParticleSystem->BuildEmitters();
		FAssetRegistryModule::AssetCreated(NewParticleSystem);
		NewParticleSystem->PostEditChange();
		NewParticleSystem->SetupSoloing();
		NewParticleSystem->Modify();
		
		Result = NewParticleSystem;
		
	}
	PresentObject = Result;
	return Result;
}
