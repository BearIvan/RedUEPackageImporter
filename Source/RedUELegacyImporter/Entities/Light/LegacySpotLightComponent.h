#pragma once
#include "Entities/Light/LegacyPointLightComponent.h"
#include "LegacySpotLightComponent.generated.h"

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySpotLightComponent: public ULegacyPointLightComponent
{
	GENERATED_BODY()
public:
	
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;

	UPROPERTY(BlueprintReadWrite)
	float InnerConeAngle;
	
	UPROPERTY(BlueprintReadWrite)
	float OuterConeAngle;
	
	UPROPERTY(BlueprintReadWrite)
	float LightShaftConeAngle;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyRotator Rotation;
};
