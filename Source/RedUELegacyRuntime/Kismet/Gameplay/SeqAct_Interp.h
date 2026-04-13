#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "LevelSequenceActor.h"
#include "SeqAct_Interp.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_Interp : public USequenceAction
{
	GENERATED_BODY()
public:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Play"))
	void Play();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Reverse"))
	void Reverse();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Stop"))
	void Stop();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Pause"))
	void Pause();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Change Dir"))
	void Change();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Set Position"))
	void SetPosition();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Get Position and Duration"))
	void GetPositionAndDuration();

	UPROPERTY(BlueprintAssignable,meta=(LegacyName = "Completed"))
	FSequenceActionDelegate Completed;

	UPROPERTY(BlueprintAssignable,meta=(LegacyName = "Reversed"))
	FSequenceActionDelegate Reversed;
	
	UPROPERTY(BlueprintAssignable,meta=(LegacyName = "Position Set"))
	FSequenceActionDelegate PositionSet;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Position and Duration Retrieved"))
	FSequenceActionDelegate PositionAndDurationRetrieved;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Stopped"))
	FSequenceActionDelegate Stopped;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyName = "Data"))
	ALevelSequenceActor* Data;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Data);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyName = "New Position"))
	float NewPosition;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(NewPosition);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyName = "Current Position"))
	float CurrentPosition;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(CurrentPosition);

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyName = "Duration"))
	float Duration;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Duration)

private:
	
	UFUNCTION()
	void OnFinished();
	
};

