#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_ToggleHidden.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_ToggleHidden : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Hide();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void UnHide();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Toggle();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Actors;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Actors);
};
