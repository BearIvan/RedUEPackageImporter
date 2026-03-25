#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_TeleportElizabethToPlayer.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_TeleportElizabethToPlayer: public USequenceAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};
