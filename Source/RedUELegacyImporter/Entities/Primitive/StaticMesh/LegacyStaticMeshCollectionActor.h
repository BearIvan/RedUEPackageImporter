#pragma once

#include "LegacyStaticMeshComponent.h"
#include "Core/LegacyObject.h"
#include "Entities/LegacyActor.h"
#include "LegacyStaticMeshCollectionActor.generated.h"


enum class ELegacyBioshockCollisionType : uint8
{
	COLLIDE_CustomDefault = 0x0,
	COLLIDE_NoCollision = 0x1,
	COLLIDE_DecalsOnly = 0x2,
	COLLIDE_BlockAll = 0x3,
	COLLIDE_BlockWeapons = 0x4,
	COLLIDE_TouchAll = 0x5,
	COLLIDE_TouchWeapons = 0x6,
	COLLIDE_BlockAllButWeapons = 0x7,
	COLLIDE_TouchAllButWeapons = 0x8,
	COLLIDE_BlockWeaponsKickable = 0x9,
	COLLIDE_BlockAllButRigidBody = 0xA,
	COLLIDE_MAX = 0xB,
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyStaticMeshCollectionActor : public ULegacyActor
{
	GENERATED_BODY()

public:
	virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;
	virtual AActor* Spawn_Implementation() override;

	UPROPERTY(BlueprintReadWrite)
	TArray<	ULegacyStaticMeshComponent*> StaticMeshComponents;

	ELegacyBioshockCollisionType BioCollisionType = ELegacyBioshockCollisionType::COLLIDE_CustomDefault;
};