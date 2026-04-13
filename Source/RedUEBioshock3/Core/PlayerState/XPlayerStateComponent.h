#pragma once

#include "Components/ActorComponent.h"
#include "XPlayerStateComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REDUEBIOSHOCK3_API UXPlayerStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UXPlayerStateComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Bioshock|PlayerState")
	void DPVAddCount(FName Name,int32 Count);
	
	UFUNCTION(BlueprintCallable, Category = "Bioshock|PlayerState")
	int32 DPVGetCount(FName Name) const;
	
	UFUNCTION(BlueprintCallable, Category = "Bioshock|PlayerState")
	void DPVResetCount(FName Name);
	
	UPROPERTY(VisibleAnywhere, Category = "Bioshock|PlayerState")
	TMap<FName,int32> DPVCounters;
};
