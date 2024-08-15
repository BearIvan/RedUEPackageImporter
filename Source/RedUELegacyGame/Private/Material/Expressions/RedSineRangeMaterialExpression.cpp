#include "Material/Expressions/RedSineRangeMaterialExpression.h"
#include "MaterialCompiler.h"

URedSineRangeMaterialExpression::URedSineRangeMaterialExpression(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	Max=1.0f;
	Period=1.0f;
}

int32 URedSineRangeMaterialExpression::Compile(FMaterialCompiler* Compiler, int32 OutputIndex)
{
	return  Compiler->Clamp( Compiler->Sine(Period > 0.0f ? Compiler->Mul(Compiler->GameTime(true, Period), Compiler->Constant(2.0f * PI / Period)) : Compiler->GameTime(true, 2.0f * PI)), Compiler->Constant(Min), Compiler->Constant(Max));
}

void URedSineRangeMaterialExpression::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("RedUE Legacy SineRange"));
}
