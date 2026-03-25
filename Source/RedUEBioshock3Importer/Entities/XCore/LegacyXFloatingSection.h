#pragma once

#include "Entities/LegacyActor.h"
#include "LegacyXFloatingSection.generated.h"

/**
 * 
 */
UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXFloatingSection : public ULegacyActor
{
	GENERATED_BODY()
public:
	virtual void	FillActor_Implementation		(AActor* InActor) override;
	virtual UClass* GetActorClass_Implementation	() override;
	
	UPROPERTY(BlueprintReadWrite)
	FGuid FloatingSectionGuid;

	UPROPERTY(BlueprintReadWrite)
	FLegacyMatrix InvInitialTransform;
};
