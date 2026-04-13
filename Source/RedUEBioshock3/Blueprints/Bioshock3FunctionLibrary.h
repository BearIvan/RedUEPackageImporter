#pragma once

#include "Bioshock3FunctionLibrary.generated.h"

class UXPlayerStateComponent;
/**
 * 
 */
UCLASS()
class REDUEBIOSHOCK3_API UBioshock3FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,Category = "Bioshock3")
	static UXPlayerStateComponent* GetXPlayerStateComponent(APlayerController* Player);
};
