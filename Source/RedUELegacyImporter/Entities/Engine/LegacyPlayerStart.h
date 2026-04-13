#pragma once

#include "LegacyNavigationPoint.h"
#include "Entities/LegacyActor.h"
#include "UObject/Object.h"
#include "LegacyPlayerStart.generated.h"

/**
 * 
 */

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyPlayerStart : public ULegacyNavigationPoint
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation() override;
};