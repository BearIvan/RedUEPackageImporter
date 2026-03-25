#pragma once

#include "Core/LegacyObject.h"
#include "LegacyXSound.generated.h"



UCLASS()
class REDUELEGACY_API ULegacyXAKAudioEventID : public ULegacyObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int32 Id;
	
	virtual UObject* ExportToContent() override;
};


UCLASS()
class REDUELEGACY_API ULegacyXEffectSound : public ULegacyObject
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyXAKAudioEventID* PlayEvent;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyXAKAudioEventID* StopEvent;

};

UCLASS()
class REDUELEGACY_API ULegacyXEffectSpeechPostEvent: public ULegacyXEffectSound
{
	GENERATED_BODY()
public:

};
