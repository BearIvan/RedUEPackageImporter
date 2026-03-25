#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_SetInt.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_SetInt: public USequenceAction
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(LegacyRead, KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<int32> Value;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Value);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	TArray<int32> Target;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Target);
};
