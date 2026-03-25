#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_CameraFade.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_CameraFade: public USequenceAction
{
	GENERATED_BODY()
public:
					USeqAct_CameraFade	();
	virtual void	Tick				(float DeltaTime) override;
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate Finished;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Targets;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Targets)
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FadeOpacity;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FadeTime;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BeginFadeOpacity;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FColor FadeColor;
	
private:

	UPROPERTY()
	APlayerController* CachePC;
};
