#pragma once

#include "LegacyActor.h"
#include "LegacyMaterialInstanceActor.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACY_API ULegacyMaterialInstanceActor : public ULegacyActor
{
	GENERATED_BODY()
public:
	virtual void FillActor_Implementation(AActor* InActor) override;
	virtual UClass* GetActorClass_Implementation() override;

	UPROPERTY(BlueprintReadWrite)
	class ULegacyMaterialInstanceConstant* MatInst;
	
};
