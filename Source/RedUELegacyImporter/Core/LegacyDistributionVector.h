#pragma once

#include "Core/LegacyObject.h"
#include "Entities/LegacyActorComponent.h"
#include "LegacyDistributionVector.generated.h"

/**
 * 
 */

USTRUCT()
struct FLegacyRawDistributionVector:public FLegacyRawDistribution
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (LegacyRead))
	ULegacyObject* Distribution;
	
	UDistributionVector* MakeDistributionVector(UObject*Outer) const;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyDistributionVector : public ULegacyComponent
{
	GENERATED_BODY()
};
