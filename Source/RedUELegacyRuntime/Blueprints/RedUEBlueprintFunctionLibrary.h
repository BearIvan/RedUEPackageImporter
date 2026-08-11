#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RedUEBlueprintFunctionLibrary.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API URedUEBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Red|UELegacy",meta=(WorldContext="WorldContextObject"))
	static void RemoteEventFromLevelScript(FName EventName,UObject* WorldContextObject);
};
