#pragma once
#include "Entities/Light/LegacyLightComponent.h"
#include "LegacyPointLightComponent.generated.h"

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyPointLightComponent: public ULegacyLightComponent
{
	GENERATED_BODY()
public:
	
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;

	UPROPERTY(BlueprintReadWrite)
	FVector3f Translation;

	UPROPERTY(BlueprintReadWrite)
	float Radius = 1024;
	
	UPROPERTY(BlueprintReadWrite)
	float FalloffExponent = 2;
};
