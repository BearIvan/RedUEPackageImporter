#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_PlaySound.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_PlaySound : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Play"))
	void Play();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Stop"))
	void Stop();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Out"))
	FSequenceActionDelegate Out;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Finished"))
	FSequenceActionDelegate Finished;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Stopped"))
	FSequenceActionDelegate Stopped;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "BeforeEnd"))
	FSequenceActionDelegate BeforeEnd;
	
	/** Sound cue to play on the targeted actor(s) */
	UPROPERTY(EditAnywhere, meta=(LegacyRead))
	USoundCue* PlaySound;

	/** Additional dead space to append to SoundDuration */
	UPROPERTY(EditAnywhere, meta=(LegacyRead))
	float ExtraDelay;
	
	/** Time taken for sound to fade in when action is activated. */
	UPROPERTY(EditAnywhere, meta=(LegacyRead))
	float	FadeInTime;

	/** Time take for sound to fade out when Stop input is fired. */
	UPROPERTY(EditAnywhere, meta=(LegacyRead))
	float	FadeOutTime;

	/** Volume multiplier propagated to audio component */
	UPROPERTY(EditAnywhere, meta=(LegacyRead))
	float	VolumeMultiplier = 1;

	/** Pitch multiplier propagated to audio component */
	UPROPERTY(EditAnywhere, meta=(LegacyRead))
	float	PitchMultiplier = 1;

	/** How long before end should the BeforeEnd output be activated. */
	UPROPERTY(EditAnywhere, meta=(LegacyRead))
	float	BeforeEndTime;

	/** TRUE to suppress display of any subtitles the soundcue may have.  FALSE for normal subtitle behavior. */
	UPROPERTY(EditAnywhere, meta=(LegacyRead))
	bool	bSuppressSubtitles;
	
	
};
