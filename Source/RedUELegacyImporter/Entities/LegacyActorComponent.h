#pragma once
#include "Core/LegacyObject.h"
#include "LegacyActorComponent.generated.h"


UCLASS(Blueprintable)
class REDUELEGACYIMPORTER_API ULegacyActorComponent : public ULegacyComponent
{
    GENERATED_BODY()
public:
    
    UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ReUE|Legacy")
    void FillComponent(UActorComponent* InActorComponent);
    
    UPROPERTY(BlueprintReadWrite)
    bool bAutoActivate = true;
};
