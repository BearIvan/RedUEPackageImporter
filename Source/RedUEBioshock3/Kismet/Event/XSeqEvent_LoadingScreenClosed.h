#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqEvent_LoadingScreenClosed.generated.h"

UENUM()
enum ELegacyRequiredLoadingScreenType:int32
{
	ERLST_Any,                      // 0
	ERLST_Checkpoint,               // 1
	ERLST_NonCheckpoint,            // 2
	ERLST_MAX                       // 3
};

UCLASS()
class REDUEBIOSHOCK3_API UXSeqEvent_LoadingScreenClosed : public USequenceEvent
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
