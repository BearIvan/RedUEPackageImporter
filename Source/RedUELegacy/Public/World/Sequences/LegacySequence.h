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
			UObject*	ImportKismet		(bool Reimport = false);
			void		FillActor			(ALegacyKismet* LevelKismet);
			void		GenerateBlueprint	(ULegacyKismetBlueprint* InBlueprint,UEdGraph* EventGraph);
	virtual UObject*	ExportToContent		() override;
			bool		SortNode			(class UEdGraphNode* Start, TSet<class UEdGraphNode*>& Nodes, float& Y, float X);
	

	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacySequenceObject*> SequenceObjects;
	
	ULegacySequenceVariable* FindSequenceVariable(FName InName);
};
