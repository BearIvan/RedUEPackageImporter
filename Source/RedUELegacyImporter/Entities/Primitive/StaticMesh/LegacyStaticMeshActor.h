#pragma once
#include "Entities/LegacyActor.h"
#include "LegacyStaticMeshActor.generated.h"

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyStaticMeshActorBase : public ULegacyActor
{
    GENERATED_BODY()
public:
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyStaticMeshActor : public ULegacyStaticMeshActorBase
{
    GENERATED_BODY()
public:
    virtual void        FillActor_Implementation        (AActor* InActor) override;
    virtual UClass*     GetActorClass_Implementation    () override;

    UPROPERTY(BlueprintReadWrite)
    class ULegacyStaticMeshComponent* StaticMeshComponent;
};
