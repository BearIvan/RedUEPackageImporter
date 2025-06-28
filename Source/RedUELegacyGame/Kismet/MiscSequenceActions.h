#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "MiscSequenceActions.generated.h"


UCLASS()
class REDUELEGACYGAME_API USeqCond_CompareBool : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate OutTrue;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate OutFalse;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	bool InBoolean;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(bool,InBoolean);
};

UCLASS()
class REDUELEGACYGAME_API USeqAct_AndGate : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Reset();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOpen = true;
};

UCLASS()
class REDUELEGACYGAME_API USeqAct_Gate: public USequenceAction
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
UCLASS()
class REDUELEGACYGAME_API UXSeqAct_MemoryGate: public USequenceAction
{
	GENERATED_BODY()
public:
					UXSeqAct_MemoryGate	();
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

UCLASS()
class REDUELEGACYGAME_API USeqAct_SetBool: public USequenceAction
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	bool Value;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(bool,Value);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	bool OutBoolean;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(bool,OutBoolean);
};