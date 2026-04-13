#pragma once

#include "Core/LegacyDistributionFloat.h"
#include "LegacyDistributionFloatConstantCurve.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyDistributionFloatConstantCurve : public ULegacyDistributionFloat
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (LegacyRead))
	FInterpCurveFloat ConstantCurve;
};
