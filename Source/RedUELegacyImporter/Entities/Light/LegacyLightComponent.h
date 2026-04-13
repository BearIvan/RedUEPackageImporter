#pragma once
#include "Entities/LegacyActorComponent.h"
#include "LegacyLightComponent.generated.h"

class ULegacyMaterialInterface;

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyLightFunction : public ULegacyObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	ULegacyMaterialInterface* SourceMaterial;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyLightComponent : public ULegacyActorComponent
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
