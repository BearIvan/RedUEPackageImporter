#pragma once
#include "Kismet/Base/SequenceAction.h"

#include "SeqAct_ActivateRemoteEvent.generated.h"

UCLASS()
class REDUELEGACYGAME_API USeqAct_ActivateRemoteEvent : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Broadcast();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName EventName;
};

UCLASS()
class REDUELEGACYGAME_API UXSeqAct_ActivateRemoteEvent : public USeqAct_ActivateRemoteEvent
{
	GENERATED_BODY()
};
