#pragma once

#include "Entities/Light/LegacyLight.h"
#include "LegacyPointLight.generated.h"

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyPointLight : public ULegacyLight
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation	() override;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyPointLightToggleable : public ULegacyPointLight
{
	GENERATED_BODY()
};
