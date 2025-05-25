#pragma once

#include "LegacyActorComponent.h"
#include "LegacyLightComponent.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyLightComponent : public ULegacyActorComponent
{
	GENERATED_BODY()
public:
	
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;
	
	UPROPERTY(BlueprintReadWrite)
	float Brightness;
	
	UPROPERTY(BlueprintReadWrite)
	FColor LightColor;
};

UCLASS()
class REDUELEGACY_API ULegacyPointLightComponent: public ULegacyLightComponent
{
	GENERATED_BODY()
public:
	
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;

	UPROPERTY(BlueprintReadWrite)
	FVector3f Translation;

	UPROPERTY(BlueprintReadWrite)
	float Radius;

	
};

UCLASS()
class REDUELEGACY_API ULegacySpotLightComponent: public ULegacyPointLightComponent
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
