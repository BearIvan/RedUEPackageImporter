#pragma once

#include "LegacyActorComponent.h"
#include "LegacyPrimitiveComponent.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyPrimitiveComponent : public ULegacyActorComponent
{
	GENERATED_BODY()

public:
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;

	UPROPERTY(BlueprintReadWrite)
	FVector3f Scale3D = {1,1,1};
    
	UPROPERTY(BlueprintReadWrite)
	FLegacyRotator Rotation;
    
	UPROPERTY(BlueprintReadWrite)
	FVector3f Translation;
	
};
