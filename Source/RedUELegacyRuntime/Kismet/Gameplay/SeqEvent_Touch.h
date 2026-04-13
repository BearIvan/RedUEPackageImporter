#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqEvent_Touch.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqEvent_Touch : public USequenceEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Touched"))
	FSequenceActionDelegate Touched;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "UnTouched"))
	FSequenceActionDelegate UnTouched;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "UnTouched"))
	FSequenceActionDelegate Empty;
	
	UPROPERTY(EditAnywhere)
	AActor* Originator;
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "Instigator"))
	AActor* Instigator;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Instigator);
};
