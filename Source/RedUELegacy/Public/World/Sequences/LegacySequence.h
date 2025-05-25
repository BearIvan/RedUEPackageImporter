#pragma once
#include "Object/LegacySequenceObjects.h"
#include "LegacySequence.generated.h"
class ULegacyKismetBlueprint;
class ALegacyKismet;

UCLASS()
class REDUELEGACY_API ULegacySequence : public ULegacySequenceOp
{
	GENERATED_BODY()
public:

	void FillActor(ALegacyKismet* LevelKismet);
	void GenerateBlueprint(ULegacyKismetBlueprint* InBlueprint,UEdGraph* EventGraph);
	virtual UObject* ExportToContent() override;

	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacySequenceObject*> SequenceObjects;
};
