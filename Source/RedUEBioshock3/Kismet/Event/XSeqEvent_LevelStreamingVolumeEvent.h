#pragma once

#include "CoreMinimal.h"
#include "Kismet/Base/SequenceAction.h"
#include "XSeqEvent_LevelStreamingVolumeEvent.generated.h"

/**
 * 
 */
UCLASS()
class REDUEBIOSHOCK3_API UXSeqEvent_LevelStreamingVolumeEvent : public USequenceEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Started;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate Finished;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2))
	FSequenceActionDelegate Canceled;
};
