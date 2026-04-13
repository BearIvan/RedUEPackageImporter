#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_PlayScriptedSequence.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_PlayScriptedSequence : public USequenceAction
{
	GENERATED_BODY()
public:
	
	virtual void Tick(float DeltaTime) override;;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Initiate();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Cleanup();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate SequenceReady;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate SequenceCleanedUp;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Targets;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Targets);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1))
	AActor* StartPosition;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(StartPosition);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 2))
	AActor* LookAtActor;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(LookAtActor);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 3))
	AActor* HandsMeshTargets;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(HandsMeshTargets)

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MovePlayerDuration = 0.f;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool CleanupAfterBeingReady = false;
private:
	UPROPERTY()
	float CurrentTime = 0.f;
};


UCLASS()
class REDUEBIOSHOCK3_API UXDLC2SeqAct_PlayScriptedSequence : public UXSeqAct_PlayScriptedSequence
{
	GENERATED_BODY()
public:
};