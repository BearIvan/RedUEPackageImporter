#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_SetObject.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_SetObject: public USequenceAction
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	AActor* Value;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Value);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	TArray<AActor*> Targets;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Targets);
};
