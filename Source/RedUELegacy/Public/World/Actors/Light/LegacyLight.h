#pragma once

#include "World/Actors/LegacyActor.h"
#include "LegacyLight.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyLight : public ULegacyActor
{
	GENERATED_BODY()
public:
	
	virtual void	FillActor_Implementation		(AActor* InActor) override;

	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyLightComponent* LightComponent;
};

UCLASS()
class REDUELEGACY_API ULegacySpotLight : public ULegacyLight
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation	() override;
};


UCLASS()
class REDUELEGACY_API ULegacySpotLightToggleable : public ULegacySpotLight
{
	GENERATED_BODY()
};
