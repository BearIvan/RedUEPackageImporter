#pragma once

#include "Entities/LegacyActor.h"
#include "UObject/Object.h"
#include "LegacyPlayerStart.generated.h"

/**
 * 
 */

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyPlayerStart : public ULegacyActor
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation() override;
};