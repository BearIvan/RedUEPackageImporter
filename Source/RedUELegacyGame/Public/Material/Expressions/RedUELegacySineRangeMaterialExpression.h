#pragma once

#include "RedUELegacySineRangeMaterialExpression.generated.h"

UCLASS()
class REDUELEGACYGAME_API URedUELegacySineRangeMaterialExpression : public UMaterialExpression
{
	GENERATED_BODY()
public:
					URedUELegacySineRangeMaterialExpression	(const FObjectInitializer& ObjectInitializer);
#if WITH_EDITOR
	virtual int32	Compile									(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	virtual void	GetCaption								(TArray<FString>& OutCaptions) const override;
#endif
	
	UPROPERTY(EditAnywhere, Category = MaterialExpressionAdd, meta = (OverridingInputProperty = "Period"))
	float Period;

	UPROPERTY(EditAnywhere, Category = MaterialExpressionAdd, meta = (OverridingInputProperty = "Max"))
	float Max;

	UPROPERTY(EditAnywhere, Category = MaterialExpressionAdd, meta = (OverridingInputProperty = "Min"))
	float Min;


};
