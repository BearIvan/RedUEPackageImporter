#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqEvent_Used.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqEvent_Used : public USequenceEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Used"))
	FSequenceActionDelegate Used;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Unused"))
	FSequenceActionDelegate Unused;
	
	UPROPERTY(EditAnywhere)
	AActor* Originator;
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "Instigator"))
	AActor* Instigator;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Instigator);
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "Distance"))
	float Distance;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Distance);
};
