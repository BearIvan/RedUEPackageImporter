#pragma once

#include "LegacyActor.h"
#include "LegacyBrush.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACY_API ULegacyBrush : public ULegacyActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	class ULegacyBrushComponent* BrushComponent;
	
	virtual void	FillActor_Implementation		(AActor* InActor) override;
};
