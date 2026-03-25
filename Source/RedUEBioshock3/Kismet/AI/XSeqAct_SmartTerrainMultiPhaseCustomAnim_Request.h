#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_SmartTerrainMultiPhaseCustomAnim_Request.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_SmartTerrainMultiPhaseCustomAnim_Request : public USequenceAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void SendRequests();

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> RequestsToSend;

	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Targets;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Targets);
};
