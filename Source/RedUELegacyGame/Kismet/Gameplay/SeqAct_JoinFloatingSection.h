#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_JoinFloatingSection.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYGAME_API USeqAct_JoinFloatingSection : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};
