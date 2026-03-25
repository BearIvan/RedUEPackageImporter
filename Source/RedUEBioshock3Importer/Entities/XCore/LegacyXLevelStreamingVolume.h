#pragma once

#include "LegacyXBaseLevelStreamingVolume.h"
#include "LegacyXLevelStreamingVolume.generated.h"

/**
 * 
 */
struct FXLevelAndStreamingStatus;

UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXLevelStreamingVolume : public ULegacyXBaseLevelStreamingVolume
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
