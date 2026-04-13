#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqEvent_CheckpointLoaded.generated.h"

/**
 * 
 */
UCLASS()
class REDUEBIOSHOCK3_API UXSeqEvent_CheckpointLoaded : public USequenceEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};
