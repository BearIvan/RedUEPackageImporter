#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_PlaySound.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_PlaySound: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Play();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Stop();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate Finished;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2))
	FSequenceActionDelegate Stopped;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Actors;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Actors);
	
	UPROPERTY(EditAnywhere)
	USoundBase* PlaySound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* StopSound;
	
};
