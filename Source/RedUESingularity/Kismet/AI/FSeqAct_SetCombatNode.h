#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "FSeqAct_SetCombatNode.generated.h"

/**
 * 
 * 
 */
UCLASS()
class REDUESINGULARITY_API UFSeqAct_SetCombatNode : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Set Node"))
	void SetNode();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Remove Fightstyle"))
	void RemoveFightstyle();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Out"))
	FSequenceActionDelegate Out;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Node Reached End"))
	FSequenceActionDelegate NodeReachedEnd;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Node Exited"))
	FSequenceActionDelegate NodeExited;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Fightstyle Exited"))
	FSequenceActionDelegate FightstyleExited;
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "Target"))
	TArray<AActor*> Target;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Target);
};
