#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_MemoryGate.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API UXSeqAct_MemoryGate: public USequenceAction
{
	GENERATED_BODY()
public:
	UXSeqAct_MemoryGate();
	virtual void	BeginPlay			() override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Open();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Close();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 3))
	void Toggle();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 4))
	void Clear();
	
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
