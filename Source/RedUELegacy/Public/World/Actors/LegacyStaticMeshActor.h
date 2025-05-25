#pragma once

#include "LegacyActor.h"
#include "LegacyStaticMeshActor.generated.h"

UCLASS()
class REDUELEGACY_API ULegacyStaticMeshActor : public ULegacyActor
{
    GENERATED_BODY()
public:
    virtual void FillActor_Implementation(AActor* InActor) override;
    virtual UClass* GetActorClass_Implementation() override;

    UPROPERTY(BlueprintReadWrite)
    class ULegacyStaticMeshComponent* StaticMeshComponent;

};

UCLASS()
class REDUELEGACY_API ULegacyXReactiveDynamicSMActor : public ULegacyStaticMeshActor
{
    GENERATED_BODY()
public:
};
UCLASS()
class REDUELEGACY_API ULegacyXReactiveInterpActor : public ULegacyXReactiveDynamicSMActor
{
    GENERATED_BODY()
public:
};