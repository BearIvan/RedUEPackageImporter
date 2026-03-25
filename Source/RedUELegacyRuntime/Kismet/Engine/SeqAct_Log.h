#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_Log.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_Log: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};
