#include "AssetDefinition_LegacyKismetBlueprint.h"

#include "Kismet2/BlueprintEditorUtils.h"
#include "UI/LegacyKismetBlueprintEditor.h"


EAssetCommandResult UAssetDefinition_LegacyKismetBlueprint::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	
	EToolkitMode::Type Mode = OpenArgs.ToolkitHost.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (ULegacyKismetBlueprint* Blueprint : OpenArgs.LoadObjects<ULegacyKismetBlueprint>())
	{
		TSharedRef< FLegacyKismetBlueprintEditor > NewCurveAssetEditor( new FLegacyKismetBlueprintEditor() );
		TArray<UBlueprint*> Blueprints;
		Blueprints.Add(Blueprint);
		NewCurveAssetEditor->InitKismetBlueprintEditor(Mode, OpenArgs.ToolkitHost, Blueprints, FBlueprintEditorUtils::ShouldOpenWithDataOnlyEditor(Blueprint));
	}

	return EAssetCommandResult::Handled;
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_LegacyKismetBlueprint::GetAssetCategories() const
{
	static const auto Categories = { FAssetCategoryPath(FText::FromString(TEXT("Legacy")))};
	return Categories;
}
