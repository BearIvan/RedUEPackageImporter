
#include "LegacySequence.h"

#include "BlueprintCompilationManager.h"
#include "BlueprintEditorLibrary.h"
#include "K2Node_Composite.h"
#include "ObjectTools.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/LegacyPackage.h"
#include "Engine/LevelScriptBlueprint.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

UObject* ULegacySequence::ImportKismet(bool Reimport)
{
	if(PresentObject)
	{
		return PresentObject;
	}
	
	UClass* KismetClass = ALegacyKismet::StaticClass();
	if (LegacyPackage->GameType == ERedUELegacyGameType::Bioshock3)
	{
		KismetClass = ABioshockKismet::StaticClass(); 
	}
	ULevelScriptBlueprint* LevelScriptBlueprint = GWorld->GetCurrentLevel()->GetLevelScriptBlueprint(false);
	UBlueprintEditorLibrary::ReparentBlueprint(LevelScriptBlueprint,KismetClass);
	
	FString GraphName = GetLegacyName()+TEXT("_Graph");
	if( ULevelScriptBlueprint* LegacyKismetBlueprint = CastChecked<ULevelScriptBlueprint>(LevelScriptBlueprint))
	{
		LegacyKismetBlueprint->PreEditChange(nullptr);
		if (UEdGraph* EventGraph = FindObject<UEdGraph>(LegacyKismetBlueprint, GraphName))
		{
			FBlueprintEditorUtils::RemoveGraph(LegacyKismetBlueprint,EventGraph);
		}
		
		
		if (UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(LegacyKismetBlueprint,*GraphName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass()))
		{
			FBlueprintEditorUtils::AddUbergraphPage(LegacyKismetBlueprint, NewGraph);
		
			GenerateBlueprint(LegacyKismetBlueprint,NewGraph);
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(LegacyKismetBlueprint);
		}
		
		FKismetEditorUtilities::GenerateBlueprintSkeleton(LegacyKismetBlueprint, true);
		LegacyKismetBlueprint->Modify();
		LegacyKismetBlueprint->PostEditChange();
		FBlueprintCompilationManager::CompileSynchronously(FBPCompileRequest(LegacyKismetBlueprint, EBlueprintCompileOptions::SkeletonUpToDate, nullptr));
		FAssetRegistryModule::AssetCreated(LegacyKismetBlueprint);
		
		PresentObject = nullptr;
	}
	
	FillActor(GetMutableDefault<ALegacyKismet>(LevelScriptBlueprint->GeneratedClass));
	
	return PresentObject;
}

void ULegacySequence::FillActor(ALegacyKismet* LevelKismet)
{
	for(ULegacySequenceObject* SequenceObject : SequenceObjects)
	{
		if(ULegacySequenceVariable* SequenceVariable = Cast<ULegacySequenceVariable>(SequenceObject))
		{
			SequenceVariable->Fill(LevelKismet);
		}
	}
}

void ULegacySequence::GenerateBlueprint(UBlueprint* InBlueprint,UEdGraph* EventGraph)
{
	for(ULegacySequenceObject* SequenceObject : SequenceObjects)
	{
		if (SequenceObject)
		{
			SequenceObject->ExportToBlueprint(InBlueprint,EventGraph);
		}
	}

	float X = 0;
	float Y = 0;
	TSet<class UEdGraphNode*> LinkedNodes;
	TSet<class UEdGraphNode*> SortedNodes;
	for (UEdGraphNode* Node : EventGraph->Nodes)
	{
		GetLinkedNodes(Node,LinkedNodes);
		
		for (UEdGraphNode* LinkNode : LinkedNodes)
		{
			bool bContinue = false;
			for (UEdGraphPin*Pin: LinkNode->Pins)
			{
				if (Pin->Direction == EGPD_Input&&Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec&&Pin->LinkedTo.Num()>0)
				{
					bContinue = true;
				}
			}
			if (bContinue)
			{
				continue;
			}
		
			SortNode(LinkNode,SortedNodes,Y,X);
		}
	}
}

UK2Node* ULegacySequence::ExportToBlueprint(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (CurrentNode)
	{
		return CurrentNode;
	}
	
	if (SequenceObjects.IsEmpty())
	{
		return nullptr;
	}
	
	FGraphNodeCreator<UK2Node_Composite> NodeCreator(*InGraph);
	UK2Node_Composite* CompositeNode = NodeCreator.CreateNode();

	CompositeNode->bCommentBubblePinned = true;
	CompositeNode->NodeComment = GetLegacyFullName();
	//CompositeNode->OnUpdateCommentText(GetLegacyFullName());
	NodeCreator.Finalize();
	
	{
		// Rename the graph to the correct name
		UEdGraph* DestinationGraph = CompositeNode->BoundGraph;
		TSharedPtr<INameValidatorInterface> NameValidator = MakeShareable(new FKismetNameValidator(InBlueprint, GetLegacyFName()));
		FBlueprintEditorUtils::RenameGraphWithSuggestion(DestinationGraph, NameValidator, GetLegacyName());
	}
	
	FEdGraphPinType ExecPinType(UEdGraphSchema_K2::PC_Exec, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	
	for (int32 i = 0; i < InputLinks.Num(); i++)
	{
		const FLegacySeqOpInputLink& InputLink = InputLinks[i];
		if (ULegacySeqEvent_SequenceActivated* SeqEvent_SequenceActivated = Cast<ULegacySeqEvent_SequenceActivated>(InputLink.LinkedOp))
		{
			FName InputName = InputLink.XLinkName;
			if (InputName == NAME_Name)
			{
				InputName = *InputLink.LinkDesc;
			}
			
			UEdGraphPin* InputPin =  CompositeNode->CreateUserDefinedPin(InputName,ExecPinType, EGPD_Input, true);
			
			SeqEvent_SequenceActivated->OutputPinName = InputPin->GetFName();
			SeqEvent_SequenceActivated->CurrentTunnelNode = CompositeNode->InputSinkNode;
			LegacyIndexToInputPin.Add(i, SeqEvent_SequenceActivated->OutputPinName );
		}
	}
	
	TMap<int32, FName> LegacyIndexToOutputPin;
	for (int32 i = 0; i < OutputLinks.Num(); i++)
	{
		const FLegacySeqOpOutputLink& OutputLink = OutputLinks[i];
		if (ULegacySeqAct_FinishSequence* SeqAct_FinishSequence = Cast<ULegacySeqAct_FinishSequence>(OutputLink.LinkedOp))
		{
			FName InputName = OutputLink.XLinkName;
			if (InputName == NAME_Name)
			{
				InputName = *OutputLink.LinkDesc;
			}
			
			UEdGraphPin* InputPin =  CompositeNode->CreateUserDefinedPin(InputName,ExecPinType, EGPD_Output, true);
			
			SeqAct_FinishSequence->InputPinName = InputPin->GetFName();
			SeqAct_FinishSequence->CurrentTunnelNode = CompositeNode->OutputSourceNode;
			LegacyIndexToOutputPin.Add(i, SeqAct_FinishSequence->InputPinName );
		}
	}
	
	TMap<FName, ULegacySeqVar_External*> ExternalVarMap;
	for(ULegacySequenceObject* SequenceObject : SequenceObjects)
	{
		if (ULegacySeqVar_External* ExternalVar = Cast<ULegacySeqVar_External>(SequenceObject))
		{
			ExternalVarMap.Add(ExternalVar->GetLegacyFName(), ExternalVar);
		}
	}
	
	for (int32 i = 0; i < VariableLinks.Num(); i++)
	{
		const FLegacySeqVarLink& VarLink = VariableLinks[i];
		if (ULegacySeqVar_External** ExternalVar = ExternalVarMap.Find(VarLink.LinkVar))
		{
			(*ExternalVar)->Variables = VarLink.LinkedVariables;
		}
	}
	
	CompositeNode->ReconstructNode();
	GenerateBlueprint(InBlueprint,CompositeNode->BoundGraph);
	CurrentNode = CompositeNode;
	
	for (int32  i = 0;i<OutputLinks.Num();i++)
	{
		const FLegacySeqOpOutputLink& LegacySeqOpOutputLink = OutputLinks[i];
		if (FName*OutputName = LegacyIndexToOutputPin.Find(i))
		{
			UEdGraphPin*OutputPin = CompositeNode->FindPinChecked(*OutputName);

			FillPin(InBlueprint, InGraph, LegacySeqOpOutputLink, OutputPin);
		}
	}
	
	return CompositeNode;
}

UEdGraphPin* ULegacySequence::GetInputPin(int32 Index, UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	if (CurrentNode)
	{
		FName *InputName = LegacyIndexToInputPin.Find(Index);
		if (ensure(InputName))
		{
			return CurrentNode->FindPinChecked(*InputName);
		}
	}
	return nullptr;
}

UObject* ULegacySequence::ExportToContent()
{
	return ImportKismet(false);
}

bool ULegacySequence::SortNode(class UEdGraphNode* Start, TSet<class UEdGraphNode*>& Nodes, float& Y, float X)
{
	if (!Start)return false;
	if (Nodes.Contains(Start))return false;
	Nodes.Add(Start);
	Start->NodePosX = X;
	Start->NodePosY = Y;
	float LocalY = Y;
	for (UEdGraphPin*Pin: Start->Pins)
	{
		if (Pin->Direction == EGPD_Output&&Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec&&Pin->LinkedTo.Num()>0)
		{
			SortNode(Pin->LinkedTo[0]->GetOwningNode(), Nodes, LocalY, X + 500);
		}
	}
	Y = FMath::Max(Y + 300, LocalY);
	return true;
}

void ULegacySequence::GetLinkedNodes(class UEdGraphNode* Start, TSet<class UEdGraphNode*>& LinkedNodes)
{
	if (!Start)
	{
		return;
	}
	if (LinkedNodes.Contains(Start))
	{
		return;
	}
	LinkedNodes.Add(Start);
	for (UEdGraphPin*Pin: Start->Pins)
	{
		if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec&&Pin->LinkedTo.Num()>0)
		{
			for (UEdGraphPin* Item :Pin->LinkedTo)
			{
				GetLinkedNodes(Item->GetOwningNode(), LinkedNodes);
			}
		}
	}
}

ULegacySequenceVariable* ULegacySequence::FindSequenceVariable(FName InName)
{
	for (ULegacySequenceObject*SequenceObject: SequenceObjects)
	{
		if (ULegacySequenceVariable* SequenceVariable = Cast<ULegacySequenceVariable>(SequenceObject))
		{
			if (SequenceVariable->VarName == InName)
			{
				return SequenceVariable;
			}
		}
		else if (ULegacySequence* LegacySequence = Cast<ULegacySequence>(SequenceObject))
		{
			if (ULegacySequenceVariable* Result = LegacySequence->FindSequenceVariable(InName))
			{
				return Result;
			}
		}
	}
	return nullptr;
}
