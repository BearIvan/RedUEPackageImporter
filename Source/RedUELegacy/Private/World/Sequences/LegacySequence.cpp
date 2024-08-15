#include "World/Sequences/LegacySequence.h"

#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

void ULegacySequence::GenerateBlueprint(UBlueprint* InBlueprint)
{
	UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(InBlueprint, GetLegacyFName(), UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
	NewGraph->bAllowDeletion = true;

	for(ULegacySequenceObject* SequenceObject : SequenceObjects)
	{
		if(ULegacySequenceEvent* SequenceEvent = Cast<ULegacySequenceEvent>(SequenceObject))
		{
			SequenceEvent->ExportToBlueprint(InBlueprint,NewGraph);
		}
	}
	int32 InOutNodePosY = 0;
	//FKismetEditorUtilities::AddDefaultEventNode(InBlueprint, NewGraph, GetLegacyName(), InBlueprint->ParentClass, InOutNodePosY);

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(InBlueprint);
	FBlueprintEditorUtils::AddUbergraphPage(InBlueprint, NewGraph);

	FKismetEditorUtilities::CompileBlueprint(InBlueprint, EBlueprintCompileOptions::SkipGarbageCollection);
}
