#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_PatternMatcherSetGlobalFlag.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_PatternMatcherSetGlobalFlag : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Set();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Unset();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Toggle();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FName> FlagNames;
};
