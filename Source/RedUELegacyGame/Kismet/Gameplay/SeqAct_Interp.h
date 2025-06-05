#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "LevelSequenceActor.h"
#include "SeqAct_Interp.generated.h"

UCLASS()
class REDUELEGACYGAME_API USeqAct_Interp : public USequenceAction
{
	GENERATED_BODY()
public:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Play();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Reverse();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Stop();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 3))
	void Pause();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 4))
	void Change();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 5))
	void SetPosition();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 6))
	void GetPositionAndDuration();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Completed;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate Reversed;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2))
	FSequenceActionDelegate PositionSet;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 3))
	FSequenceActionDelegate PositionAndDurationRetrieved;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 4))
	FSequenceActionDelegate Stopped;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0))
	ALevelSequenceActor* Data;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ALevelSequenceActor*,Data);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1))
	float NewPosition;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(float,NewPosition);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 2))
	float CurrentPosition;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(float,CurrentPosition);

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 3))
	float Duration;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(float,Duration)

private:
	
	UFUNCTION()
	void OnFinished();
	
};

