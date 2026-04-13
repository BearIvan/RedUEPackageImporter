#pragma once

#include "Entities/LegacyActor.h"
#include "LegacyLight.generated.h"

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyLight : public ULegacyActor
{
	GENERATED_BODY()
public:
	
	virtual void	FillActor_Implementation		(AActor* InActor) override;

	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyLightComponent* LightComponent;
};