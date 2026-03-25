#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_WaitForLevelsVisible.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_WaitForLevelsVisible: public USequenceAction
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Wait();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Finished;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FName> LevelNames;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bShouldBlockOnLoad;
	
	UPROPERTY(Transient)
	TArray<ULevelStreaming*> Levels;
};
