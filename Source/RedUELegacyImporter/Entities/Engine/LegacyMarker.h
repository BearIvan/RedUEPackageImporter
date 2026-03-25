#pragma once

#include "Entities/LegacyActor.h"
#include "LegacyMarker.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyMarker : public ULegacyActor
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation() override;
	
};

