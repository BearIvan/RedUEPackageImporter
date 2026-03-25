#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_EndScriptBehavior.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_EndScriptBehavior : public USequenceAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void In();

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Targets;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Targets);
};
