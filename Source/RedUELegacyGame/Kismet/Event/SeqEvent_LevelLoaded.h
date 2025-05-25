#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqEvent_LevelLoaded.generated.h"



UCLASS()
class REDUELEGACYGAME_API USeqEvent_LevelLoaded : public USequenceAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate LoadedAndVisible;

	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate MatchHasBegun;

	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2))
	FSequenceActionDelegate LevelReset;
	
};
