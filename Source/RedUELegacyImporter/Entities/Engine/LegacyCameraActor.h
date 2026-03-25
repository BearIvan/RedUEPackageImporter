#pragma once

#include "Entities/LegacyActor.h"
#include "LegacyCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyCameraActor : public ULegacyActor
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation() override;
	
};