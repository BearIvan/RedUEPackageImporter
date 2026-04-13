#pragma once

#include "LegacyRvActorComponent.h"
#include "Entities/LegacyActorComponent.h"
#include "LegacyRvComponents.generated.h"

/**
 * 
 */
UCLASS()
class REDUESINGULARITYIMPORTER_API ULegacyRvComponents : public ULegacyActorComponent
{
	GENERATED_BODY()
public:
	
	
	UFUNCTION(BlueprintCallable)
	ULegacyRvActorComponent*  GetRvComponent(ERvComponentType Type) const;
	virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;

	UPROPERTY(meta=(LegacyRead,ArraySizeEnum = ERvComponentType))
	ULegacyRvActorComponent* mComponents[static_cast<int32>(ERvComponentType::ERCT_NUM)];
	
};
