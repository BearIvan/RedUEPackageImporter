#pragma once

#include "Entities/LegacyActorComponent.h"
#include "LegacyRvActorComponent.generated.h"

UENUM(Blueprintable)
enum class ERvComponentType:uint8
{
	ERCT_MaterialConstants,         // 0
	ERCT_ManipulationGlow,          // 1
	ERCT_Destruction,               // 2
	ERCT_Handle,                    // 3
	ERCT_Age,                       // 4
	ERCT_Heat,                      // 5
	ERCT_Anachronism,               // 6
	ERCT_Deadlockable,              // 7
	ERCT_Plug,                      // 8
	ERCT_Receptacle,                // 9
	ERCT_Ladder,                    // 10
	ERCT_Disintegrator,             // 11
	ERCT_DamageTypeScalars,         // 12
	ERCT_AutoAim,                   // 13
	ERCT_HealthBlocks,              // 14
	ERCT_E99,                       // 15
	ERCT_Hexplate,                  // 16
	ERCT_TimeSink,                  // 17
	ERCT_Globule,                   // 18
	ERCT_GlobSpawner,               // 19
	ERCT_PhaseCyst,                 // 20
	ERCT_AttributeModManager,       // 21
	ERCT_Jitter,                    // 22
	ERCT_PostProcess,               // 23
	ERCT_Use,                       // 24
	ERCT_LifeLink,                  // 25
	ERCT_Synchronize,               // 26
	ERCT_ProjectilePath,            // 27
	ERCT_PossessedSounds,           // 28
	ERCT_AttackStream,              // 29
	ERCT_DamageTypeScalarsToOthers, // 30
	ERCT_Weapon,                    // 31
	ERCT_NUM,
};
/**
 * 
 */
UCLASS()
class REDUESINGULARITYIMPORTER_API ULegacyRvActorComponent : public ULegacyActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ERvComponentType mComponentType = ERvComponentType::ERCT_NUM;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mActorGroup;
};
