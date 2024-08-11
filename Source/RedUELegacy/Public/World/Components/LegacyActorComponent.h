#pragma once
#include "Core/LegacyObject.h"
#include "LegacyActorComponent.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyActorComponent : public ULegacyObject
{
    GENERATED_BODY()
public:
    virtual void LegacySerializeComponent(FArchive& Ar) override;
    virtual bool IsAComponent() override { return true; }

    UPROPERTY(BlueprintReadWrite)
    FVector3f Scale3D = {1,1,1};
    
    UPROPERTY(BlueprintReadWrite)
    FRotator3f Rotation;
    
    UPROPERTY(BlueprintReadWrite)
    FVector3f Translation;
};
