#pragma once

#include "Entities/Engine/LegacyCameraActor.h"
#include "LegacyXMatineeCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXMatineeCameraActor : public ULegacyCameraActor
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation	() override;
	virtual void	FillActor_Implementation		(AActor* InActor) override;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacySkeletalMeshComponent* SkeletalMeshComponent;
};
