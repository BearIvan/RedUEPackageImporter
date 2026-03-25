#pragma once
#include "Object/LegacySequenceObjects.h"
#include "LegacySequence.generated.h"
class ULegacyKismetBlueprint;
class ALegacyKismet;

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySequence : public ULegacySequenceOp
{
	GENERATED_BODY()
public:
	
	virtual UK2Node*				ExportToBlueprint	(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual UEdGraphPin*			GetInputPin			(int32 Index,UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual UObject*				ExportToContent		() override;
			UObject*				ImportKismet		(bool Reimport = false);
			void					FillActor			(ALegacyKismet* LevelKismet);
			void					GenerateBlueprint	(UBlueprint* InBlueprint,UEdGraph* EventGraph);
			bool					SortNode			(class UEdGraphNode* Start, TSet<class UEdGraphNode*>& Nodes, float& Y, float X);
			void					GetLinkedNodes		(class UEdGraphNode* Start, TSet<class UEdGraphNode*>& LinkedNodes);
	

	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacySequenceObject*> SequenceObjects;
	
	ULegacySequenceVariable* FindSequenceVariable(FName InName);
	
	TMap<int32, FName> LegacyIndexToInputPin;
};
