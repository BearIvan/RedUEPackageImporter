#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqEvent_Console.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqEvent_Console: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ConsoleEventName;
};
