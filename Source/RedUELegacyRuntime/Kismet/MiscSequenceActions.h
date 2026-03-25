#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "MiscSequenceActions.generated.h"


UCLASS()
class REDUELEGACYRUNTIME_API USeqCond_CompareBool : public USequenceAction
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

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(InBoolean);
};


UCLASS()
class REDUELEGACYRUNTIME_API USeqCond_CompareFloat : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0), meta = (DisplayName = "ValueA <= ValueB"))
	FSequenceActionDelegate OutA;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1), meta = (DisplayName = "ValueA > ValueB"))
	FSequenceActionDelegate OutB;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2), meta = (DisplayName = "ValueA == ValueB"))
	FSequenceActionDelegate OutC;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 3), meta = (DisplayName = "ValueA < ValueB"))
	FSequenceActionDelegate OutD;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 4), meta = (DisplayName = "ValueA >= ValueB"))
	FSequenceActionDelegate OutE;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0))
	float ValueA;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ValueA);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1))
	float ValueB;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ValueB);
};


UCLASS()
class REDUELEGACYRUNTIME_API USeqCond_CompareInt : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0), meta = (DisplayName = "ValueA <= ValueB"))
	FSequenceActionDelegate OutA;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1), meta = (DisplayName = "ValueA > ValueB"))
	FSequenceActionDelegate OutB;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2), meta = (DisplayName = "ValueA == ValueB"))
	FSequenceActionDelegate OutC;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 3), meta = (DisplayName = "ValueA < ValueB"))
	FSequenceActionDelegate OutD;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 4), meta = (DisplayName = "ValueA >= ValueB"))
	FSequenceActionDelegate OutE;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0))
	int32 ValueA;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ValueA);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1))
	int32 ValueB;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ValueB);
};


UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_AndGate : public USequenceAction
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
UCLASS()
class REDUELEGACYRUNTIME_API UXSeqAct_MemoryGate: public USequenceAction
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
class REDUELEGACYRUNTIME_API USeqAct_SetBool: public USequenceAction
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta = (LegacyRead, LegacyIndex = 0))
	bool Value = false;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<bool> Values;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Values);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	TArray<bool> Target;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Target);
};

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_SetFloat: public USequenceAction
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(LegacyRead, KismetExternalVariable ,LegacyIndex = 0))
	TArray<float> Value;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Value);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	TArray<float> Target;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Target);
};


UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_SetInt: public USequenceAction
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(LegacyRead, KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<int32> Value;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Value);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	TArray<int32> Target;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Target);
};


UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_SetString: public USequenceAction
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(LegacyRead, KismetExternalVariable ,LegacyIndex = 0 ))
	FString Value;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Value);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	FString Target;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Target);
};




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