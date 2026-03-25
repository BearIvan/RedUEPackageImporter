#pragma once

#include "LegacyActorComponent.h"
#include "LegacyLightComponent.generated.h"

class ULegacyMaterialInterface;

UCLASS()
class REDUELEGACY_API ULegacyLightFunction : public ULegacyObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	ULegacyMaterialInterface* SourceMaterial;
};

UCLASS()
class REDUELEGACY_API ULegacyLightComponent : public ULegacyActorComponent
{
	GENERATED_BODY()
public:
	
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;
	
	UPROPERTY(BlueprintReadWrite)
	float Brightness = 1.f;
	
	UPROPERTY(BlueprintReadWrite)
	FColor LightColor = FColor::White;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyLightFunction* Function;
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
	float Radius = 1024;
	
	UPROPERTY(BlueprintReadWrite)
	float FalloffExponent = 2;

	
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
