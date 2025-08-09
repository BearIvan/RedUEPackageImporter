#pragma once

#include "MaterialInstanceHybridActor.generated.h"

UCLASS()
class REDUELEGACYGAME_API AMaterialInstanceHybridActor : public AActor
{
	GENERATED_BODY()

public:
	AMaterialInstanceHybridActor();
	
	UPROPERTY(EditInstanceOnly,Interp,Category = "Legacy|MaterialInstance",BlueprintReadWrite)
	class UMaterialInstanceHybrid* MaterialInstance;

	
};
