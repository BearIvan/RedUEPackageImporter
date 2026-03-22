#pragma once
#include "SequenceAction.h"
#include "SeqAct_TestAction.generated.h"

UCLASS(MinimalAPI)
class USeqAct_TestAction : public USequenceAction
{
	GENERATED_BODY()

	
public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Test1();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput))
	void Test2();

	UPROPERTY(BlueprintAssignable)
	FSequenceActionDelegate OnFinish;

	UPROPERTY(BlueprintAssignable)
	FSequenceActionDelegate OnTest2;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ))
	bool Test123;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Test123);
	
	UPROPERTY(EditAnywhere)
	float TimerInterval = 1;
	
	UPROPERTY(EditAnywhere)
	float Timer = 0;

};


UCLASS(MinimalAPI)
class USeqAct_TestAction2 : public USequenceAction
{
	GENERATED_BODY()

	
public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Test1();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput))
	void Test2();

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ))
	bool Test123;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Test123);
	
	UPROPERTY(EditAnywhere)
	float TimerInterval = 1;
	
	UPROPERTY(EditAnywhere)
	float Timer = 0;
	
	
	UPROPERTY(EditAnywhere,meta=(KismetLinkCount))
	int32 LinkCount = 1;

};

