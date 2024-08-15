#pragma once

#include "Core/LegacyObject.h"
#include "Object/LegacySequenceObjects.h"
#include "LegacySequence.generated.h"

UCLASS()
class REDUELEGACY_API ULegacySequence : public ULegacySequenceOp
{
	GENERATED_BODY()
public:

	void GenerateBlueprint(UBlueprint* InBlueprint);
	
	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacySequenceObject*> SequenceObjects;
};
