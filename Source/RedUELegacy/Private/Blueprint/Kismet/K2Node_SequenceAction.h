#pragma once

#include "K2Node_BaseAsyncTask.h"
#include "K2Node_SequenceAction.generated.h"

class USequenceAction;
UCLASS()
class REDUELEGACY_API UK2Node_SequenceAction : public UK2Node
{
	GENERATED_BODY()
public:
										UK2Node_SequenceAction		(const FObjectInitializer& ObjectInitializer);

	// UEdGraphNode interface
	virtual void 						GetMenuActions				(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void 						AllocateDefaultPins			() override;
	virtual void 						ExpandNode					(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual bool 						IsCompatibleWithGraph		(const UEdGraph* Graph) const override;
	virtual bool 						ShouldShowNodeProperties	() const override { return true; }
	virtual FText						GetNodeTitle				(ENodeTitleType::Type TitleType) const override;
	virtual bool						IsNodeRootSet				() const override;
	virtual class FNodeHandlingFunctor* CreateNodeHandler			(class FKismetCompilerContext& CompilerContext) const override;
	virtual TSharedPtr<SGraphNode>		CreateVisualWidget			() override;

			bool						CanAddEventPin				() const;
			void						AddEventPin					();
			void						CreateEventPins				();
			void						CreateEventPin				();
			void						RemoveEventPin				(UEdGraphPin* InGraphPin);
			void						GetEventPins				(TArray<UEdGraphPin*>& OutPins);
			UEdGraphPin*				GetEventOutput				();
			


	UPROPERTY(VisibleAnywhere,Instanced)
	USequenceAction* Action;

	UPROPERTY()
	TMap<int32,FName> LegacyIndexToInputPin;
	
	UPROPERTY()
	TMap<int32,FName> LegacyIndexToOutputPin;
	
	UPROPERTY()
	TMap<int32,FName> LegacyIndexToVariableName;

	UPROPERTY()
	FName InputEventsName;

	UPROPERTY()
	int32 EventPinsCount = 0;
};
