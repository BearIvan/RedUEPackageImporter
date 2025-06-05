#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_InstancePattern.generated.h"


UCLASS()
class REDUELEGACYGAME_API UXSeqAct_InstancePattern : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void DestroyInstance();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate PatternInstanced;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2))
	FSequenceActionDelegate PatternFailed;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 3))
	FSequenceActionDelegate PatternMatched;
	
};
