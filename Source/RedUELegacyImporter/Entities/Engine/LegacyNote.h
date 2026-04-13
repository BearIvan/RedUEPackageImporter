#pragma once

#include "Entities/LegacyActor.h"
#include "LegacyNote.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyNote : public ULegacyActor
{
	GENERATED_BODY()
public:
	virtual UClass* GetActorClass_Implementation() override;
	virtual void FillActor_Implementation(AActor* InActor) override;

	UPROPERTY(EditAnywhere,meta = (LegacyRead))
	FString Text;
};
