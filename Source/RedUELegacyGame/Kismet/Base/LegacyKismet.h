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
