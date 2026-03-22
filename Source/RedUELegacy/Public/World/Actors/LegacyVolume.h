#pragma once

#include "LegacyBrush.h"
#include "LegacyVolume.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACY_API ULegacyVolume : public ULegacyBrush
{
	GENERATED_BODY()
};


UCLASS()
class REDUELEGACY_API ULegacyXBaseLevelStreamingVolume : public ULegacyVolume
{
	GENERATED_BODY()
public:
};

struct FXLevelAndStreamingStatus;

UCLASS()
class REDUELEGACY_API ULegacyXLevelStreamingVolume : public ULegacyXBaseLevelStreamingVolume
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, meta = (LegacyRead))
	TArray<FXLevelAndStreamingStatus> Levels;
	
	UPROPERTY(EditAnywhere, meta = (LegacyRead))
	bool bLevelStreamingActive = true;
	
	virtual UClass* GetActorClass_Implementation	() override;
	virtual void	FillActor_Implementation(AActor* InActor) override;
};
