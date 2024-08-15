#pragma once

#include "RedMaterialInstanceActor.generated.h"

UCLASS()
class REDUELEGACYGAME_API ARedMaterialInstanceActor : public AActor
{
	GENERATED_BODY()

public:
	ARedMaterialInstanceActor();

	UFUNCTION(BlueprintCallable,Category = "Red|UELegacy|MaterialInstance")
	void SetScalarParameter(FName ParameterName,float Scalar);
	
	UFUNCTION(BlueprintCallable,Category = "Red|UELegacy|MaterialInstance")
	void SetVectorParameter(FName ParameterName,const FVector4f& Vector);
	
	UPROPERTY(EditInstanceOnly,Category = "Red|UELegacy|MaterialInstance",BlueprintReadWrite)
	UMaterialInstanceConstant* MaterialInstance;
};
