#pragma once

#include "LegacyBrush.h"
#include "LegacyVolume.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyVolume : public ULegacyBrush
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyTriggerVolume : public ULegacyBrush
{
	GENERATED_BODY()

public:
	virtual UClass* GetActorClass_Implementation() override;
};

