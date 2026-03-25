#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_GetDistance.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_GetDistance: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0))
	AActor* A;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(A);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1))
	AActor* B;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(B);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 2))
	float Distance;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Distance);
};
