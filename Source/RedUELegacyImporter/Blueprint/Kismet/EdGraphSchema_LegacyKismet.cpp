// Fill out your copyright notice in the Description page of Project Settings.


#include "EdGraphSchema_LegacyKismet.h"

#include "K2Node_SequenceAction.h"
#include "LegacyKismetGraphEditorCommands.h"
#include "ToolMenu.h"
#include "Kismet2/BlueprintEditorUtils.h"

void UEdGraphSchema_LegacyKismet::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);

	const UEdGraph* CurrentGraph = Context->Graph;
	const UEdGraphNode* InGraphNode = Context->Node;
	const UEdGraphPin* InGraphPin = Context->Pin;
	const bool bIsDebugging = Context->bIsDebugging;
	check(CurrentGraph);
	UBlueprint* OwnerBlueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(CurrentGraph);

	if (InGraphPin)
	{
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("EdGraphSchemaPinActions");
			if (!bIsDebugging)
			{	
				if (InGraphNode->IsA(UK2Node_SequenceAction::StaticClass()))
				{
					Section.AddMenuEntry( FLegacyKismetGraphEditorCommands::Get().RemoveEventPin );
				}
	
			}
		}
	}
}
