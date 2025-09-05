#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RedXPawnInterface.generated.h"

UINTERFACE()
class URedXPawnInterface : public UInterface
{
	GENERATED_BODY()
};

class REDUELEGACYGAME_API IRedXPawnInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Bioshock|XPawn")
	void SetSmartTerrain(FName Name);

	UFUNCTION(BlueprintImplementableEvent, Category = "Bioshock|XPawn")
	void SendCurrentSmartTerrain(const TArray<FName> &RequestsToSend);

	UFUNCTION(BlueprintImplementableEvent, Category = "Bioshock|XPawn")
	void Speak(USoundBase* Audio);

	UFUNCTION(BlueprintImplementableEvent, Category = "Bioshock|XPawn")
	bool IsSpeaking();
	
};
