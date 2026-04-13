#pragma once

#include "LegacyRvComponent_UseStation.h"
#include "LegacyRvComponent_TreasureContainer.generated.h"

USTRUCT(Blueprintable)
struct FRvSpawnPoint
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector3f mLocation = {};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator3f mRotation = {};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mBoneOrSocket;
};

class ULegacyActor;

UCLASS()
class REDUESINGULARITYIMPORTER_API ULegacyRvComponent_TreasureContainer : public ULegacyRvComponent_UseStation
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULegacyObject* mConstantSpawns = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULegacyObject* mRandomSpawns = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULegacyActor* mRandSpawnsRefArch = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbDestroyOnLevelStart = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbSpawnOnDestroy = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbSpawnOnActiveStage = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbSpawnOnLevelStart = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbBiasAmmoTowardsHeldWeapons = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbSpawn1950sVersions = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbFillHealthOnUse = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int mActiveStage = -1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float mRespawnInterval = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FRvSpawnPoint> mSpawnPoints;
};
