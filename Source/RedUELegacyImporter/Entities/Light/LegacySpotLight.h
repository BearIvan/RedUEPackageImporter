#pragma once

#include "Entities/Light/LegacyLight.h"
#include "LegacySpotLight.generated.h"

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySpotLight : public ULegacyLight
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation	() override;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacySpotLightToggleable : public ULegacySpotLight
{
	GENERATED_BODY()
};
