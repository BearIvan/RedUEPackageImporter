#include "AssetDefinition_MaterialInstanceHybrid.h"

#include "Toolkits/SimpleAssetEditor.h"

TConstArrayView<FAssetCategoryPath> UAssetDefinition_MaterialInstanceHybrid::GetAssetCategories() const
{
	static const auto Categories = { FAssetCategoryPath(FText::FromString(TEXT("Legacy")))};
	return Categories;
}

EAssetCommandResult UAssetDefinition_MaterialInstanceHybrid::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	TSharedRef<FSimpleAssetEditor> Editor = FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, OpenArgs.ToolkitHost, OpenArgs.LoadObjects<UObject>());
	Editor->SetPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateStatic([]() -> bool
	{
		return true;
	}));

	return EAssetCommandResult::Handled;
}
