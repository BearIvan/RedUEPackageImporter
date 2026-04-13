#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqCond_CompareInt.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqCond_CompareInt : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	virtual void In();
	
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
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable, LegacyRead ,LegacyIndex = 0))
	int32 ValueA;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ValueA);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable, LegacyRead ,LegacyIndex = 1))
	int32 ValueB;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ValueB);
};

UCLASS()
class REDUELEGACYRUNTIME_API USeqCond_Increment : public USeqCond_CompareInt
{
	GENERATED_BODY()
public:
	virtual void In() override;
	
	UPROPERTY(EditAnywhere,meta=(LegacyRead))
	int32 IncrementAmount = 1;
};
