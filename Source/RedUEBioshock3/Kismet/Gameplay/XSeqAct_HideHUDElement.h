#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_HideHUDElement.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_HideHUDElement : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void HideElements();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void ShowElements();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};
