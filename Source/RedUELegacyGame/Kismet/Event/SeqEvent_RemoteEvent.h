#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqEvent_RemoteEvent.generated.h"

UCLASS()
class REDUELEGACYGAME_API USeqEvent_RemoteEvent : public USequenceAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName EventName;
};

UCLASS()
class REDUELEGACYGAME_API UXSeqEvent_RemoteEvent : public USeqEvent_RemoteEvent
{
	GENERATED_BODY()
};