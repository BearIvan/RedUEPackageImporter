#pragma once

#include "Entities/LegacyActor.h"
#include "LegacyNavigationPoint.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyNavigationPoint : public ULegacyActor
{
	GENERATED_BODY()
public:
	
	virtual UClass* GetActorClass_Implementation() override;
};
