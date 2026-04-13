#pragma once

#include "Entities/LegacyActor.h"
#include "LegacyStaticLightCollectionActor.generated.h"

class ULegacyLightComponent;

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyStaticLightCollectionActor : public ULegacyActor
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
