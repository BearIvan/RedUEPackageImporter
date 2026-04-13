#pragma once

#include "LegacyXAKAudioID.h"
#include "Core/LegacyObject.h"
#include "Core/LegacyXEffect.h"
#include "LegacyXSound.generated.h"




UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXEffectSound : public ULegacyXEffect
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyXAKAudioEventID* PlayEvent;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyXAKAudioEventID* StopEvent;

};

UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXEffectSpeechPostEvent: public ULegacyXEffectSound
{
	GENERATED_BODY()
public:

};
