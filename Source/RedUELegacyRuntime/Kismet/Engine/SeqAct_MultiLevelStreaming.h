#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_MultiLevelStreaming.generated.h"

USTRUCT(Blueprintable)
struct FLegacyLevelStreamingNameCombo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName LevelName;
	
	UPROPERTY(Transient)
	ULevelStreaming* LevelStreaming;
};

/**
 * 
 */
UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_MultiLevelStreaming : public USequenceAction
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Construct() override;
	
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Load();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Unload();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Finished;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FLegacyLevelStreamingNameCombo> Levels;
};
