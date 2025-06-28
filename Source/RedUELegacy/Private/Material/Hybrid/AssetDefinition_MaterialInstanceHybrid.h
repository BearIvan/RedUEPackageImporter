#pragma once

#include "AssetDefinition.h"
#include "Materials/Hybrid/MaterialInstanceHybrid.h"
#include "AssetDefinition_MaterialInstanceHybrid.generated.h"


UCLASS()
class REDUELEGACY_API UAssetDefinition_MaterialInstanceHybrid : public UAssetDefinition
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override { return FText::FromString("Material Instance Hybrid"); }
	virtual FLinearColor GetAssetColor() const override { return FLinearColor(FColor(78, 40, 165)); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UMaterialInstanceHybrid::StaticClass(); }

	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
};
