#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_PlayerAbilityFilter.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_PlayerAbilityFilter : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};
