#pragma once

#include "AssetDefinition.h"
#include "LegacyKismetBlueprint.h"
#include "AssetDefinition_LegacyKismetBlueprint.generated.h"

UCLASS()
class REDUELEGACY_API UAssetDefinition_LegacyKismetBlueprint : public UAssetDefinition
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override { return FText::FromString("Legacy Kismet"); }
	virtual FLinearColor GetAssetColor() const override { return FLinearColor(FColor(78, 40, 165)); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return ULegacyKismetBlueprint::StaticClass(); }

	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};
