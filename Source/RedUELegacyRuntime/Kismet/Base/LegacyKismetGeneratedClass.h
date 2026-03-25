#pragma once

#include "Engine/BlueprintGeneratedClass.h"
#include "LegacyKismetGeneratedClass.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API ULegacyKismetGeneratedClass : public UBlueprintGeneratedClass
{
	GENERATED_BODY()
public:
	
	virtual void PurgeClass(bool bRecompilingOnLoad) override;
	
	UPROPERTY()
	TMap<FGuid,USequenceAction*> StartupActions;
};
