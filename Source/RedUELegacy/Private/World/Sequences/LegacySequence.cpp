#include "World/Sequences/LegacySequence.h"

#include "BlueprintCompilationManager.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/Kismet/LegacyKismetBlueprint.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Kismet/Base/LegacyKismetGeneratedClass.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

void ULegacySequence::FillActor(ALegacyKismet* LevelKismet)
{
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
}

UObject* ULegacySequence::ExportToContent()
{
	if(PresentObject)
	{
		return PresentObject;
	}
	const FString ObjectPath = GetOutContentPath()/ GetLegacyFullName().Replace(TEXT("."),TEXT("/"));
	const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
	const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
	ULegacyKismetBlueprint* LegacyKismetBlueprint = LoadObject<ULegacyKismetBlueprint>(nullptr, *FullObjectPath,nullptr,LOAD_NoWarn);
	if(!LegacyKismetBlueprint)
	{
		UPackage* InParent = CreatePackage(*PackageName);
		LegacyKismetBlueprint = CastChecked<ULegacyKismetBlueprint>(FKismetEditorUtilities::CreateBlueprint(ALegacyKismet::StaticClass(), InParent,  * FPaths::GetBaseFilename(PackageName), BPTYPE_Normal, ULegacyKismetBlueprint::StaticClass(), ULegacyKismetGeneratedClass::StaticClass(), NAME_None));
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
	}
	PresentObject = LegacyKismetBlueprint;
	return LegacyKismetBlueprint;
}
