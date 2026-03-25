#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqEvent_RemoteEvent.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqEvent_RemoteEvent : public USequenceEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName EventName;
	
	UPROPERTY(Transient)
	USeqEvent_RemoteEvent* NextEvent;
};