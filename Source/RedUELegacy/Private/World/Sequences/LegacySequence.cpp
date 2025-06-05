#include "World/Sequences/LegacySequence.h"

#include "BlueprintCompilationManager.h"
#include "ObjectTools.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/Kismet/LegacyKismetBlueprint.h"
#include "Core/LegacyPackage.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Kismet/Base/LegacyKismetGeneratedClass.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

UObject* ULegacySequence::ImportKismet(bool Reimport)
{
	if(PresentObject)
	{
		return PresentObject;
	}
	auto ReCreateObject = [Reimport](const FString& Path,UBlueprint*&OutNewObject,UClass* ParentClass)
	{
		UObject* ExistingAsset = nullptr;
		
		UPackage* InParent = CreatePackage(*Path);
		FName InName = *FPaths::GetBaseFilename(Path);
		// create an asset if it doesn't exist
		ExistingAsset = StaticFindObject(nullptr, InParent, *InName.ToString());
		if (!Reimport)
		{
			OutNewObject = Cast<UBlueprint>(ExistingAsset);
			return false;
		}
		if (!ExistingAsset)
		{
			OutNewObject = FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent,  * FPaths::GetBaseFilename(*Path), BPTYPE_Normal, ULegacyKismetBlueprint::StaticClass(), ULegacyKismetGeneratedClass::StaticClass(), NAME_None);
			ensure(OutNewObject);
			return true;
		}
		
		// otherwise delete and replace
		if (!ObjectTools::DeleteSingleObject(ExistingAsset))
		{
			UE_LOG(LogRedUELegacy, Warning, TEXT("Could not delete existing asset %s"), *ExistingAsset->GetFullName());
			OutNewObject = nullptr;
			return false;
		}

		// keep InPackage alive through the GC, in case ExistingAsset was the only reason it was around
		const bool bRootedPackage = InParent->IsRooted();

		if (!bRootedPackage)
		{
			InParent->AddToRoot();
		}

		// force GC so we can cleanly create a new asset (and not do an 'in place' replacement)
		CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);

		if (!bRootedPackage)
		{
			InParent->RemoveFromRoot();
		}

		// try to find the existing asset again now that the GC has occurred
		ExistingAsset = StaticFindObject(nullptr, InParent, *InName.ToString());

		// if the object is still around after GC, fail this operation
		if (ExistingAsset)
		{
			OutNewObject = nullptr;
			return false;
		}

		// create the asset in the package
		OutNewObject = FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent,  * FPaths::GetBaseFilename(*Path), BPTYPE_Normal, ULegacyKismetBlueprint::StaticClass(), ULegacyKismetGeneratedClass::StaticClass(), NAME_None);
		ensure(OutNewObject);
		return true;

	};
	
	const FString Name = FPaths::GetBaseFilename( LegacyPackage->FileName)/GetLegacyName();
	const FString ObjectPath = GetOutContentPath() / Name.Replace(TEXT("."),TEXT("/"));

	UBlueprint*ResultBlueprint = nullptr;
	UClass* KismetClass = ALegacyKismet::StaticClass();
	if (LegacyPackage->GameType == ERedUELegacyGameType::Bioshock3)
	{
		KismetClass = ABioshockKismet::StaticClass(); 
	}
	if(ReCreateObject(ObjectPath,ResultBlueprint,KismetClass))
	{
		ULegacyKismetBlueprint* LegacyKismetBlueprint = CastChecked<ULegacyKismetBlueprint>(ResultBlueprint);
		LegacyKismetBlueprint->PreEditChange(nullptr);
		if (UEdGraph* EventGraph = FindObject<UEdGraph>(LegacyKismetBlueprint, *UEdGraphSchema_K2::GN_EventGraph.ToString()))
		{
			GenerateBlueprint(LegacyKismetBlueprint,EventGraph);
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(LegacyKismetBlueprint);
		}
		FKismetEditorUtilities::GenerateBlueprintSkeleton(LegacyKismetBlueprint, true);
		LegacyKismetBlueprint->Modify();
		LegacyKismetBlueprint->PostEditChange();
		FBlueprintCompilationManager::CompileSynchronously(FBPCompileRequest(LegacyKismetBlueprint, EBlueprintCompileOptions::SkeletonUpToDate, nullptr));
		FAssetRegistryModule::AssetCreated(LegacyKismetBlueprint);
		
		PresentObject = LegacyKismetBlueprint;
	}
	else if (ResultBlueprint)
	{
		for(ULegacySequenceObject* SequenceObject : SequenceObjects)
		{
			if(ULegacySequenceOp* SequenceEvent = Cast<ULegacySequenceOp>(SequenceObject))
			{
				SequenceEvent->SimulatedImport();
			}
		}
		
		PresentObject = ResultBlueprint;
	}
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

void ULegacySequence::GenerateBlueprint(ULegacyKismetBlueprint* InBlueprint,UEdGraph* EventGraph)
{
	for(ULegacySequenceObject* SequenceObject : SequenceObjects)
	{
		if(ULegacySequenceOp* SequenceEvent = Cast<ULegacySequenceOp>(SequenceObject))
		{
			SequenceEvent->ExportToBlueprint(InBlueprint,EventGraph);
		}
	}
	
	float X = 0;
	float Y = 0;
	TSet<class UEdGraphNode*> SortedNodes;
	for (UEdGraphNode* Node : EventGraph->Nodes)
	{
		bool bContinue = false;
		for (UEdGraphPin*Pin: Node->Pins)
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
		
		if (SortNode(Node,SortedNodes,Y,X))
		{
			Y+=300;
		}
	}
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
	for (UEdGraphPin*Pin: Start->Pins)
	{
		if (Pin->Direction == EGPD_Output&&Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec&&Pin->LinkedTo.Num()>0)
		{
			if(SortNode(Pin->LinkedTo[0]->GetOwningNode(), Nodes, Y, X + 300))
				Y += 300;
		}
	}
	return true;
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
	}
	return nullptr;
}
