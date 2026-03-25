#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_JoinFloatingSection.generated.h"

/**
 * 
 */
UCLASS()
class REDUEBIOSHOCK3_API USeqAct_JoinFloatingSection : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(EditAnywhere, meta = (LegacyRead))
	FName FloatingSectionActor;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Targets;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Targets);
};
