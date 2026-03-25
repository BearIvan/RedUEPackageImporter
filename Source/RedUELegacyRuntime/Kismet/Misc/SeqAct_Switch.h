#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_Switch.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_Switch: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(EditAnywhere,meta=(KismetLinkCount, UIMin = 1, ClampMin = 1, LegacyRead))
	int32 LinkCount = 1;
	
	UPROPERTY(EditAnywhere, meta = (LegacyRead))
	int32 IncrementAmount = 1;
	
	UPROPERTY(EditAnywhere, meta = (LegacyRead))
	bool bLooping = false;
	
	UPROPERTY(EditAnywhere, meta = (LegacyRead))
	bool bAutoDisableLinks = false;
	
	UPROPERTY(EditAnywhere, meta = (LegacyRead))
	TArray<int32> Indices;
	
private:
	UPROPERTY()
	TSet<int32> DisabledIndices;
};
