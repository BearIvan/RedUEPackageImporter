#pragma once

#include "Materials/MaterialInstanceConstant.h"
#include "MaterialInstanceHybrid.generated.h"

UCLASS(EditInlineNew,DefaultToInstanced)
class REDUELEGACYGAME_API UMaterialInstanceConstant_Instanced: public UMaterialInstanceConstant
{
	GENERATED_BODY()
};

UCLASS()
class REDUELEGACYGAME_API UMaterialInstanceHybrid : public UMaterialInstanceDynamic
{
	GENERATED_BODY()
public:
					UMaterialInstanceHybrid	();
	virtual void	PostLoad				() override;
			void	InitializeMID			(class UMaterialInterface* ParentMaterial);
	
	UPROPERTY(BlueprintReadOnly,Category="Hybrid")
	UMaterialInstanceConstant_Instanced* ConstantBase;
	
};
