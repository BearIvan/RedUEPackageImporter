#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqCond_CompareFloat.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqCond_CompareFloat : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0), meta = (DisplayName = "ValueA <= ValueB"))
	FSequenceActionDelegate OutA;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1), meta = (DisplayName = "ValueA > ValueB"))
	FSequenceActionDelegate OutB;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2), meta = (DisplayName = "ValueA == ValueB"))
	FSequenceActionDelegate OutC;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 3), meta = (DisplayName = "ValueA < ValueB"))
	FSequenceActionDelegate OutD;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 4), meta = (DisplayName = "ValueA >= ValueB"))
	FSequenceActionDelegate OutE;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0))
	float ValueA;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ValueA);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1))
	float ValueB;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ValueB);
};
