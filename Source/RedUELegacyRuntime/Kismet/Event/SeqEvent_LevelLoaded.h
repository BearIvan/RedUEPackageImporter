#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqEvent_LevelLoaded.generated.h"



UCLASS()
class REDUELEGACYRUNTIME_API USeqEvent_LevelLoaded : public USequenceEvent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate LoadedAndVisible;

	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate MatchHasBegun;

	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2))
	FSequenceActionDelegate LevelReset;
	
};

UENUM()
enum ELegacyRequiredLoadingScreenType:int32
{
	ERLST_Any,                      // 0
	ERLST_Checkpoint,               // 1
	ERLST_NonCheckpoint,            // 2
	ERLST_MAX                       // 3
};

UCLASS()
class REDUELEGACYRUNTIME_API UXSeqEvent_LoadingScreenClosed : public USequenceEvent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta = (LegacyRead))
	TArray<FName> CheckpointDisplayInfoStructLookupNames;
	
	UPROPERTY(EditAnywhere,meta = (LegacyRead))
	TEnumAsByte<ELegacyRequiredLoadingScreenType> RequiredLoadingScreenType = ELegacyRequiredLoadingScreenType::ERLST_Any;
	
};


