#pragma once
#include "Entities/LegacyActor.h"
#include "LegacyBrush.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyBrush : public ULegacyActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	class ULegacyBrushComponent* BrushComponent;
	
	virtual void	FillActor_Implementation		(AActor* InActor) override;
};
