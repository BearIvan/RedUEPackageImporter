#pragma once

#include "LegacyPrimitiveComponent.h"
#include "LegacyBrushComponent.generated.h"

/**
 * 
 */
class ULegacyModel;
UCLASS()
class REDUELEGACY_API ULegacyBrushComponent : public ULegacyPrimitiveComponent
{
	GENERATED_BODY()
public:
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;
	UPROPERTY(BlueprintReadWrite)
	ULegacyModel* Brush;
};
