#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_SetBool.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_SetBool: public USequenceAction
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta = (LegacyRead, LegacyIndex = 0))
	bool Value = false;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<bool> Values;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Values);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	TArray<bool> Target;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Target);
};
