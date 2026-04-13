#pragma once

#include "Core/LegacyObject.h"
#include "LegacyXAKAudioID.generated.h"

/**
 * 
 */
UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXAKAudioID : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual UObject* ExportToContent() override;
	
	UPROPERTY(meta = (LegacyRead))
	FString AudioObject;
	
	UPROPERTY(meta = (LegacyRead))
	int32 Id;
};



UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXAKAudioEventID : public ULegacyXAKAudioID
{
	GENERATED_BODY()
public:
};