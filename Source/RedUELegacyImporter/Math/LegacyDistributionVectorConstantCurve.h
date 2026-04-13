#pragma once

#include "Core/LegacyDistributionVector.h"
#include "LegacyDistributionVectorConstantCurve.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyDistributionVectorConstantCurve : public ULegacyDistributionVector
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (LegacyRead))
	FInterpCurveVector3f ConstantCurve;
};
