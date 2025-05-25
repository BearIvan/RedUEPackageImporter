#pragma once

#include "LegacyActor.h"
#include "LegacySkeletalMeshActor.generated.h"

UCLASS()
class REDUELEGACY_API ULegacySkeletalMeshActor : public ULegacyActor
{
	GENERATED_BODY()
public:

	virtual void FillActor_Implementation(AActor* InActor) override;
	virtual UClass* GetActorClass_Implementation() override;
    
	UPROPERTY(BlueprintReadWrite)
	class ULegacySkeletalMeshComponent* SkeletalMeshComponent;
};

UCLASS()
class REDUELEGACY_API ULegacyXReactiveSkeletalMeshActor : public ULegacySkeletalMeshActor
{
	GENERATED_BODY()
public:
};