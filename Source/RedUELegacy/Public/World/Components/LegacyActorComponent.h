#pragma once
#include "Core/LegacyObject.h"
#include "LegacyActorComponent.generated.h"

UCLASS(EditInlineNew,DefaultToInstanced)
class REDUELEGACY_API ULegacyActorComponent : public ULegacyObject
{
    GENERATED_BODY()
public:
    virtual void LegacySerializeComponent(FArchive& Ar) override;
    virtual bool IsAComponent() override { return true; }
    
    UFUNCTION(BlueprintNativeEvent)
    void FillComponent( UActorComponent* InActorComponent);
};
