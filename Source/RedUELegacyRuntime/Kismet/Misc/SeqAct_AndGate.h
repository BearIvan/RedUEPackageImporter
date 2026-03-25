#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_AndGate.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_AndGate : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Reset();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOpen = true;
};
