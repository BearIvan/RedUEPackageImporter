#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_ModifyPawnMovement.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_ModifyPawnMovement : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Apply();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Remove();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};
