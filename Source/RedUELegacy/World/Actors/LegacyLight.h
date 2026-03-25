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

UCLASS()
class REDUELEGACY_API ULegacyPointLight : public ULegacyLight
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation	() override;
};


UCLASS()
class REDUELEGACY_API ULegacyPointLightToggleable : public ULegacyPointLight
{
	GENERATED_BODY()
};


UCLASS()
class REDUELEGACY_API ULegacyStaticLightCollectionActor : public ULegacyActor
{
	GENERATED_BODY()

public:
	virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;
	virtual AActor* Spawn_Implementation() override;

	UPROPERTY(BlueprintReadWrite)
	TArray<	ULegacyLightComponent*> LightComponents;
	
	UPROPERTY(Transient)
	TArray<FMatrix44f> Matrices;

};