#pragma once

#include "LegacyKismet.generated.h"

class USeqEvent_RemoteEvent;

UCLASS()
class REDUELEGACYRUNTIME_API ALegacyKismet : public AActor
{
	GENERATED_BODY()

public:
					ALegacyKismet		();
	virtual void	BeginPlay			() override;
	virtual void	Tick				(float DeltaTime) override;
	virtual void	ActivateRemoteEvent (const FName&InName);

	
	UFUNCTION(BlueprintCallable,Category="Legacy|Kismet", meta = (ComponentClass = "/Script/RedUELegacyGame.SequenceAction",DeterminesOutputType = "SequenceActionClass"))
	USequenceAction*GetSequenceAction(FGuid ActionGuid,TSubclassOf<USequenceAction> SequenceActionClass);

	UPROPERTY(VisibleInstanceOnly)
	TMap<FGuid,USequenceAction*> SequenceActions;
	
	UPROPERTY(VisibleInstanceOnly)
	TMap<FName,USeqEvent_RemoteEvent*> SequenceRemoteEvents;

	UPROPERTY(BlueprintReadOnly, Category="Kismet|Bioshock")
	APlayerController* PlayerController;
private:
	UPROPERTY(Transient)
	TArray<FName> RemoteEventNames;
	
	UPROPERTY()
	bool bFirstTick;
};



UCLASS()
class REDUELEGACYRUNTIME_API ABioshockKismet : public ALegacyKismet
{
	GENERATED_BODY()

public:
	ABioshockKismet();

	UFUNCTION(BlueprintCallable,Category="Kismet|Bioshock")
	void RefreshElizabethPawn();
	
	UPROPERTY(BlueprintReadOnly, Category="Kismet|Bioshock")
	bool ElizabethIsActive = false;

	UPROPERTY(BlueprintReadOnly, Category="Kismet|Bioshock")
	APawn* ElizabethPawn;

};
