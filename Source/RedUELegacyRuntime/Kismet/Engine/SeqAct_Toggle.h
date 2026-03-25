#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_Toggle.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_Toggle : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void TurnOn();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void TurnOff();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Toggle();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(meta = (KismetEvent,LegacyIndex = 0))
	TArray<FGuid> Event;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Actors;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Actors);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	bool Bool;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Bool);
	
};
