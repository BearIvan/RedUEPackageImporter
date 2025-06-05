#pragma once

#include "LegacyKismet.generated.h"

UCLASS()
class REDUELEGACYGAME_API ALegacyKismet : public AActor
{
	GENERATED_BODY()

public:
					ALegacyKismet	();
	virtual void	BeginPlay		() override;
	virtual void	Tick			(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category="Legacy|Kismet", meta = (ComponentClass = "/Script/RedUELegacyGame.SequenceAction",DeterminesOutputType = "SequenceActionClass"))
	USequenceAction*GetSequenceAction(FGuid ActionGuid,TSubclassOf<USequenceAction> SequenceActionClass);

	UPROPERTY(VisibleInstanceOnly)
	TMap<FGuid,USequenceAction*> SequenceActions;
};



UCLASS()
class REDUELEGACYGAME_API ABioshockKismet : public ALegacyKismet
{
	GENERATED_BODY()

public:
	ABioshockKismet();

	UPROPERTY(BlueprintReadOnly, Category="Kismet|Bioshock")
	bool ElizabethIsActive = false;

	UPROPERTY(BlueprintReadOnly, Category="Kismet|Bioshock")
	APawn* ElizabethPawn;
};
