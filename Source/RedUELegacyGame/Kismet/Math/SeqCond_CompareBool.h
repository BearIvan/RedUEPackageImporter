#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqCond_CompareBool.generated.h"


UCLASS()
class REDUELEGACYGAME_API USeqCond_CompareBool : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate OutTrue;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate OutFalse;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	bool InBoolean;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(bool,InBoolean);
};
