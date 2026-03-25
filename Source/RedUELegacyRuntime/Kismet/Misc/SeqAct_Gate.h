#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_Gate.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_Gate: public USequenceAction
{
	GENERATED_BODY()
public:
	USeqAct_Gate();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Open();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Close();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 3))
	void Toggle();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AutoCloseCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOpen = true;
private:
	UPROPERTY()
	int32 CurrentCloseCount = 0;
};
