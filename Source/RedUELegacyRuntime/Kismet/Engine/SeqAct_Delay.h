#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_Delay.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_Delay : public USequenceAction
{
	GENERATED_BODY()
public:
					USeqAct_Delay		();
	virtual void	Tick				(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Start();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Stop();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Pause();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Finished;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate Aborted;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(KismetExternalVariable ,LegacyIndex = 0))
	float Duration = 1;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Duration);
private:
	UPROPERTY()
	float CurrentTime;

	UPROPERTY()
	bool Paused;
};
