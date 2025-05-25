#pragma once
#include "Base/SequenceAction.h"
#include "SeqAct_TestAction.generated.h"

UCLASS(MinimalAPI)
class USeqAct_TestAction : public USequenceAction
{
	GENERATED_BODY()

	
public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput))
	void Test1();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput))
	void Test2();

	UPROPERTY(BlueprintAssignable)
	FSequenceActionDelegate OnFinish;

	UPROPERTY(BlueprintAssignable)
	FSequenceActionDelegate OnTest2;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ))
	bool Test123;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(bool,Test123);
	
	UPROPERTY(EditAnywhere)
	float TimerInterval = 1;
	
	UPROPERTY(EditAnywhere)
	float Timer = 0;

};
