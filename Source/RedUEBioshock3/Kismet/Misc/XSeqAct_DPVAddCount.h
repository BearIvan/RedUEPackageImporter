#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_DPVAddCount.generated.h"

/**
 * 
 */
UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_DPVAddCount : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0))
	APlayerController* Player;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Player);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable, LegacyRead ,LegacyIndex = 1))
	int32 Amount = 0;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Amount);
	
	UPROPERTY(EditAnywhere, meta = (LegacyRead))
	FName DesignerPlayerVariable;
	
};
