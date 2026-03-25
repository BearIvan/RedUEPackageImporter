#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_StopSmartTerrain.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_StopSmartTerrain : public USequenceAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void In();
	
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void RequestOutro();

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 1))
	FSequenceActionDelegate Done;
};
