#pragma once

#include "World/Sequences/Object/LegacySequenceObjects.h"
#include "LegacyXSeqAct_PlayScriptedSequence.generated.h"

UCLASS()
class ULegacyXSeqAct_PlayScriptedSequence : public ULegacySequenceAction
{
	GENERATED_BODY()
public:
	virtual UEdGraphPin* 	GetPinOrCreateNode	(UBlueprint* InBlueprint,UEdGraph* InGraph,int32 InputLinkIdx);
};
