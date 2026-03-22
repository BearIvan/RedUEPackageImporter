#pragma once

#include "GameFramework/Volume.h"
#include "XLevelStreamingVolume.generated.h"


UENUM(Blueprintable)
enum EStreamingVolumeLevelStatus
{
	SVLS_Loaded,
	SVLS_Visible,
};

USTRUCT(Blueprintable)
struct FXLevelAndStreamingStatus
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EStreamingVolumeLevelStatus> DesiredStatus = SVLS_Loaded;
};

UCLASS()
class REDUELEGACYGAME_API AXLevelStreamingVolume : public AVolume
{
	GENERATED_BODY()

public:
					AXLevelStreamingVolume	();
	virtual void	BeginPlay				() override;
	virtual void	NotifyActorBeginOverlap	(AActor* OtherActor) override;
private:
			void	OnWorldMatchStarting	();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FXLevelAndStreamingStatus> Levels;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLevelStreamingActive = true;
private:
	UPROPERTY(Transient)
	TArray<ULevelStreaming*> CachedLevelsStreaming;
};
