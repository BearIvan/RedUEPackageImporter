// Tyran


#include "Material/LegacyMaterialExpression.h"

#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacyGame.h"
#include "Material/LegacyMaterial3.h"
#include "Material/LegacyTexture3.h"
#include "Material/Expressions/RedUELegacySineRangeMaterialExpression.h"
#include "Material/Expressions/RedUELegacySineRangeMaterialExpression.h"
#include "Materials/MaterialExpressionAbs.h"
#include "Materials/MaterialExpressionAdd.h"
#include "Materials/MaterialExpressionAppendVector.h"
#include "Materials/MaterialExpressionBumpOffset.h"
#include "Materials/MaterialExpressionCameraPositionWS.h"
#include "Materials/MaterialExpressionCameraVectorWS.h"
#include "Materials/MaterialExpressionClamp.h"
#include "Materials/MaterialExpressionComponentMask.h"
#include "Materials/MaterialExpressionConstant.h"
#include "Materials/MaterialExpressionConstant2Vector.h"
#include "Materials/MaterialExpressionConstant3Vector.h"
#include "Materials/MaterialExpressionConstant4Vector.h"
#include "Materials/MaterialExpressionCustom.h"
#include "Materials/MaterialExpressionDesaturation.h"
#include "Materials/MaterialExpressionDivide.h"
#include "Materials/MaterialExpressionDotProduct.h"
#include "Materials/MaterialExpressionFloor.h"
#include "Materials/MaterialExpressionFrac.h"
#include "Materials/MaterialExpressionFresnel.h"
#include "Materials/MaterialExpressionLightVector.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"
#include "Materials/MaterialExpressionMultiply.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionOneMinus.h"
#include "Materials/MaterialExpressionPanner.h"
#include "Materials/MaterialExpressionPixelDepth.h"
#include "Materials/MaterialExpressionPower.h"
#include "Materials/MaterialExpressionReflectionVectorWS.h"
#include "Materials/MaterialExpressionRotator.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionSceneDepth.h"
#include "Materials/MaterialExpressionSine.h"
#include "Materials/MaterialExpressionSmoothStep.h"
#include "Materials/MaterialExpressionStaticComponentMaskParameter.h"
#include "Materials/MaterialExpressionStaticSwitchParameter.h"
#include "Materials/MaterialExpressionStep.h"
#include "Materials/MaterialExpressionSubtract.h"
#include "Materials/MaterialExpressionTemporalSobol.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "Materials/MaterialExpressionTextureSampleParameterCube.h"
#include "Materials/MaterialExpressionTime.h"
#include "Materials/MaterialExpressionTransform.h"
#include "Materials/MaterialExpressionTransformPosition.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionVertexColor.h"
#include "Materials/MaterialExpressionWorldPosition.h"

template <typename MaterialExpressionType>
inline MaterialExpressionType* CreateExpressionTyped(UMaterial* Material, UClass* ClassExpression = nullptr)
{
	MaterialExpressionType* Expression = NewObject<MaterialExpressionType>(Material, ClassExpression ? ClassExpression : MaterialExpressionType::StaticClass());

	if (Material->IsA<UMaterial>())
	{
		Material->GetExpressionCollection().Expressions.Add(Expression);
		Expression->Material = Material;
	}
	// MaterialFunction->FunctionExpressions.Add(NewExpression);

	Expression->UpdateMaterialExpressionGuid(true, true);

	// use for params
	Expression->UpdateParameterGuid(true, true);
	if (Expression->HasAParameterName())
	{
		Expression->ValidateParameterName(false);
	}
	if (Material->IsA<UMaterial>())
	{
		Material->AddExpressionParameter(Expression, Material->EditorParameters);
	}

	Expression->MarkPackageDirty();

	return Expression;
}

void ULegacyMaterialExpression::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	Ar.Seek(Ar.GetStopper());
}

bool ULegacyMaterialExpression::LegacySupport_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	if (EngineType != ERedUELegacyEngineType::UnrealEngine3)
	{
		return false;
	}
	return Super::LegacySupport_Implementation(EngineType, GameType);
}

UMaterialExpression* ULegacyMaterialExpression::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	ensure(false);
	return nullptr;
}

void ULegacyMaterialExpression::SetExpressionInput(FExpressionInput& Out, const FLegacyExpressionInput& In, UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!In.Expression)
	{
		return;
	}
	Out.Mask = In.Mask;
	Out.MaskR = In.MaskR;
	Out.MaskG = In.MaskG;
	Out.MaskB = In.MaskB;
	Out.MaskA = In.MaskA;
	Out.Expression = In.Expression->CreateExpression(Material,Parameters);
}

UMaterialExpression* ULegacyMaterialExpression::StartCreateExpression(UMaterial* Material, const FLegacyMaterialExpressionParameters& Parameters)
{
	if(Parameters.WorldDeformer)
	{
		UMaterialExpressionSubtract* ExpressionSubtract = CreateExpressionTyped<UMaterialExpressionSubtract>(Material);
		ExpressionSubtract->PreEditChange(nullptr);
		ExpressionSubtract->A.Expression = CreateExpression(Material,Parameters);
		{
			UMaterialExpressionWorldPosition* ExpressionWorldPosition = CreateExpressionTyped<UMaterialExpressionWorldPosition>(Material);
			ExpressionWorldPosition->PreEditChange(nullptr);
			ExpressionWorldPosition->Desc = GetLegacyFullName();
			ExpressionWorldPosition->PostEditChange();
			ExpressionWorldPosition->Modify();
			ExpressionSubtract->B.Expression = ExpressionWorldPosition;
		}
		ExpressionSubtract->Desc = GetLegacyFullName();
		ExpressionSubtract->PostEditChange();
		ExpressionSubtract->Modify();
		return ExpressionSubtract;
	}
	return CreateExpression(Material,Parameters);
}

UMaterialExpression* ULegacyMaterialExpressionTextureSample::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTextureSample* Expression = CreateExpressionTyped<UMaterialExpressionTextureSample>(Material);
		SetExpressionInput(Expression->Coordinates, Coordinates, Material,Parameters);
		if(Texture)
		{
			Expression->Texture = CastChecked<UTexture>(Texture->ExportToContent(), ECastCheckedType::NullAllowed);
		}
		SetExpressionInput(Expression->TextureObject, SamplerSource, Material,Parameters);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionTextureSampleParameter2D::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTextureSampleParameter2D* Expression = CreateExpressionTyped<UMaterialExpressionTextureSampleParameter2D>(Material);
		SetExpressionInput(Expression->Coordinates, Coordinates, Material,Parameters);;
		if(ensure(Texture))
		{
			Expression->Texture = CastChecked<UTexture>(Texture->ExportToContent(), ECastCheckedType::NullAllowed);
		}
		SetExpressionInput(Expression->TextureObject, SamplerSource, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->ParameterName = ParameterName;
		Expression->Group = Group;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionTextureSampleParameterCube::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTextureSampleParameterCube* Expression = CreateExpressionTyped<UMaterialExpressionTextureSampleParameterCube>(Material);
		SetExpressionInput(Expression->Coordinates, Coordinates, Material,Parameters);;
		if(ensure(Texture))
		{
			Expression->Texture = CastChecked<UTexture>(Texture->ExportToContent(), ECastCheckedType::NullAllowed);
		}
		SetExpressionInput(Expression->TextureObject, SamplerSource, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->ParameterName = ParameterName;
		Expression->Group = Group;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionScalarParameter::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionScalarParameter* Expression = CreateExpressionTyped<UMaterialExpressionScalarParameter>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->ParameterName = ParameterName;
		Expression->Group = Group;
		Expression->DefaultValue = DefaultValue;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionStaticBoolParameter::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionStaticBoolParameter* Expression = CreateExpressionTyped<UMaterialExpressionStaticBoolParameter>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->ParameterName = ParameterName;
		Expression->Group = Group;
		Expression->DefaultValue = DefaultValue;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionStaticSwitchParameter::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionStaticSwitchParameter* Expression = CreateExpressionTyped<UMaterialExpressionStaticSwitchParameter>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->ParameterName = ParameterName;
		Expression->Group = Group;
		Expression->DefaultValue = DefaultValue;
		SetExpressionInput(Expression->A, A, Material,Parameters);;
		SetExpressionInput(Expression->B, B, Material,Parameters);;
		if(Parameters.IsTextureDeformer)
		{
			if(!A.Expression)
			{
				UMaterialExpressionTextureCoordinate* ExpressionConstant = CreateExpressionTyped<UMaterialExpressionTextureCoordinate>(Material);
				ExpressionConstant->Desc = GetLegacyFullName();
				ExpressionConstant->PostEditChange();
				ExpressionConstant->Modify();
				Expression->A.Expression = ExpressionConstant;
			}
			if(!B.Expression)
			{
				UMaterialExpressionTextureCoordinate* ExpressionConstant = CreateExpressionTyped<UMaterialExpressionTextureCoordinate>(Material);
				ExpressionConstant->Desc = GetLegacyFullName();
				ExpressionConstant->PostEditChange();
				ExpressionConstant->Modify();
				Expression->B.Expression = ExpressionConstant;
			}
		}
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionVectorParameter::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionVectorParameter* Expression = CreateExpressionTyped<UMaterialExpressionVectorParameter>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->ParameterName = ParameterName;
		Expression->Group = Group;
		Expression->DefaultValue = DefaultValue;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionConstant::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionConstant* Expression = CreateExpressionTyped<UMaterialExpressionConstant>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->R = R;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionConstant2Vector::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionConstant2Vector* Expression = CreateExpressionTyped<UMaterialExpressionConstant2Vector>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->R = R;
		Expression->G = G;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionConstant3Vector::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionConstant3Vector* Expression = CreateExpressionTyped<UMaterialExpressionConstant3Vector>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->Constant.R = R;
		Expression->Constant.G = G;
		Expression->Constant.B = B;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionConstant4Vector::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionConstant4Vector* Expression = CreateExpressionTyped<UMaterialExpressionConstant4Vector>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->Constant.R = R;
		Expression->Constant.G = G;
		Expression->Constant.B = B;
		Expression->Constant.A = A;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionTextureCoordinate::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTextureCoordinate* Expression = CreateExpressionTyped<UMaterialExpressionTextureCoordinate>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->CoordinateIndex = CoordinateIndex;
		Expression->UTiling = UTiling;
		Expression->VTiling = VTiling;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionTransform::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTransform* Expression = CreateExpressionTyped<UMaterialExpressionTransform>(Material);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		switch (TransformSourceType)
		{
		case ELegacyMaterialVectorCoordTransformSource::TRANSFORMSOURCE_World:
			Expression->TransformSourceType = TRANSFORMSOURCE_World;
			break;
		case ELegacyMaterialVectorCoordTransformSource::TRANSFORMSOURCE_Local:
			Expression->TransformSourceType = TRANSFORMSOURCE_Local;
			break;
		case ELegacyMaterialVectorCoordTransformSource::TRANSFORMSOURCE_Tangent:
			Expression->TransformSourceType = TRANSFORMSOURCE_Tangent;
			break;
		case ELegacyMaterialVectorCoordTransformSource::TRANSFORMSOURCE_View:
			Expression->TransformSourceType = TRANSFORMSOURCE_View;
			break;
		}
		switch (TransformType)
		{
		case ELegacyMaterialVectorCoordTransform::TRANSFORM_World:
			Expression->TransformType = TRANSFORM_World;
			break;
		case ELegacyMaterialVectorCoordTransform::TRANSFORM_View:
			Expression->TransformType = TRANSFORM_View;
			break;
		case ELegacyMaterialVectorCoordTransform::TRANSFORM_Local:
			Expression->TransformType = TRANSFORM_Local;
			break;
		case ELegacyMaterialVectorCoordTransform::TRANSFORM_Tangent:
			Expression->TransformType = TRANSFORM_Tangent;
			break;
		}
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionReflectionVector::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionReflectionVectorWS* Expression = CreateExpressionTyped<UMaterialExpressionReflectionVectorWS>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionTransformPosition::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTransformPosition* Expression = CreateExpressionTyped<UMaterialExpressionTransformPosition>(Material);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		switch (TransformSourceType)
		{
		case ELegacyMaterialPositionTransformSource::TRANSFORMPOSSOURCE_Local:
			Expression->TransformSourceType = TRANSFORMPOSSOURCE_Local;
			break;
		case ELegacyMaterialPositionTransformSource::TRANSFORMPOSSOURCE_PostProjection:
			Expression->TransformSourceType = TRANSFORMPOSSOURCE_Camera;
			break;
		}
		switch (TransformType)
		{
		case ELegacyMaterialPositionTransform::TRANSFORMPOS_World:
			Expression->TransformType = TRANSFORMPOSSOURCE_World;
			break;
		}
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}


UMaterialExpression* ULegacyMaterialExpressionAdd::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionAdd* Expression = CreateExpressionTyped<UMaterialExpressionAdd>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->A, A, Material,Parameters);;
		SetExpressionInput(Expression->B, B, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionSubtract::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionSubtract* Expression = CreateExpressionTyped<UMaterialExpressionSubtract>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->A, A, Material,Parameters);;
		SetExpressionInput(Expression->B, B, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionMultiply::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionMultiply* Expression = CreateExpressionTyped<UMaterialExpressionMultiply>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->A, A, Material,Parameters);;
		SetExpressionInput(Expression->B, B, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionDivide::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionDivide* Expression = CreateExpressionTyped<UMaterialExpressionDivide>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->A, A, Material,Parameters);;
		SetExpressionInput(Expression->B, B, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionPower::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionPower* Expression = CreateExpressionTyped<UMaterialExpressionPower>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Base, Base, Material,Parameters);;
		SetExpressionInput(Expression->Exponent, Exponent, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionClamp::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionClamp* Expression = CreateExpressionTyped<UMaterialExpressionClamp>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		SetExpressionInput(Expression->Min, Min, Material,Parameters);;
		SetExpressionInput(Expression->Max, Max, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionTime::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTime* Expression = CreateExpressionTyped<UMaterialExpressionTime>(Material);
		Expression->PreEditChange(nullptr);
		Expression->bIgnorePause = bIgnorePause;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionOneMinus::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionOneMinus* Expression = CreateExpressionTyped<UMaterialExpressionOneMinus>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionFrac::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionFrac* Expression = CreateExpressionTyped<UMaterialExpressionFrac>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionLinearInterpolate::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionLinearInterpolate* Expression = CreateExpressionTyped<UMaterialExpressionLinearInterpolate>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Alpha, Alpha, Material,Parameters);;
		SetExpressionInput(Expression->A, A, Material,Parameters);;
		SetExpressionInput(Expression->B, B, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionLightVector::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UClass* MaterialExpressionLightVectorClass = LoadClass<UMaterialExpression>(nullptr,TEXT("/Script/Engine.MaterialExpressionLightVector"));
		UMaterialExpression* Expression = CreateExpressionTyped<UMaterialExpression>(Material, MaterialExpressionLightVectorClass);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionComponentMask::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionComponentMask* Expression = CreateExpressionTyped<UMaterialExpressionComponentMask>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		Expression->R = R;
		Expression->G = G;
		Expression->B = B;
		Expression->A = A;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionAbs::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionAbs* Expression = CreateExpressionTyped<UMaterialExpressionAbs>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionMeshEmitterVertexColor::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionVertexColor* Expression = CreateExpressionTyped<UMaterialExpressionVertexColor>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionPixelDepth::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionPixelDepth* Expression = CreateExpressionTyped<UMaterialExpressionPixelDepth>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionDestDepth::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UClass* MaterialExpressionSceneDepth = LoadClass<UMaterialExpression>(nullptr,TEXT("/Script/Engine.MaterialExpressionSceneDepth"));
		UMaterialExpression* Expression = CreateExpressionTyped<UMaterialExpression>(Material, MaterialExpressionSceneDepth);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionAppendVector::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionAppendVector* Expression = CreateExpressionTyped<UMaterialExpressionAppendVector>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->A, A, Material,Parameters);;
		SetExpressionInput(Expression->B, B, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionCameraVector::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTransform* Expression = CreateExpressionTyped<UMaterialExpressionTransform>(Material);
		Expression->PreEditChange(nullptr);
		{
			UMaterialExpressionCameraVectorWS* CameraVector = CreateExpressionTyped<UMaterialExpressionCameraVectorWS>(Material);
			CameraVector->PreEditChange(nullptr);
			CameraVector->Desc = GetLegacyFullName();
			CameraVector->PostEditChange();
			CameraVector->Modify();
			Expression->Input.Expression = CameraVector;
			Expression->TransformSourceType = TRANSFORMSOURCE_World;
			Expression->TransformType = TRANSFORM_Tangent;
		}
		
		CurrentExpression = Expression;
		Expression->PostEditChange();
		Expression->Modify();
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionPanner::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionPanner* Expression = CreateExpressionTyped<UMaterialExpressionPanner>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->Coordinate, Coordinate, Material,Parameters);;
		SetExpressionInput(Expression->Time, Time, Material,Parameters);;
		Expression->SpeedX = SpeedX;
		Expression->SpeedY = SpeedY;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionConstantClamp::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionClamp* Expression = CreateExpressionTyped<UMaterialExpressionClamp>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		Expression->MinDefault = Min;
		Expression->MaxDefault = Max;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionWorldPosition::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionWorldPosition* Expression = CreateExpressionTyped<UMaterialExpressionWorldPosition>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionVertexColor::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionVertexColor* Expression = CreateExpressionTyped<UMaterialExpressionVertexColor>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionFresnel::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionFresnel* Expression = CreateExpressionTyped<UMaterialExpressionFresnel>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->Exponent = Exponent;
		SetExpressionInput(Expression->Normal, Normal, Material,Parameters);;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionFloor::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionFloor* Expression = CreateExpressionTyped<UMaterialExpressionFloor>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionStaticComponentMaskParameter::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionStaticComponentMaskParameter* Expression = CreateExpressionTyped<UMaterialExpressionStaticComponentMaskParameter>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->ParameterName = ParameterName;
		Expression->Group = Group;
		Expression->DefaultA = DefaultA;
		Expression->DefaultR = DefaultR;
		Expression->DefaultG = DefaultG;
		Expression->DefaultB = DefaultB;
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionDesaturation::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionDesaturation* Expression = CreateExpressionTyped<UMaterialExpressionDesaturation>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		SetExpressionInput(Expression->Fraction, Percent, Material,Parameters);;
		Expression->LuminanceFactors = LuminanceFactors;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionBumpOffset::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionBumpOffset* Expression = CreateExpressionTyped<UMaterialExpressionBumpOffset>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->Coordinate, Coordinate, Material,Parameters);;
		SetExpressionInput(Expression->Height, Height, Material,Parameters);;
		SetExpressionInput(Expression->HeightRatioInput, HeightRatioInput, Material,Parameters);;
		Expression->HeightRatio = HeightRatio;
		Expression->ReferencePlane = ReferencePlane;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionNormalize::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionNormalize* Expression = CreateExpressionTyped<UMaterialExpressionNormalize>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->VectorInput, VectorInput, Material,Parameters);;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionDotProduct::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionDotProduct* Expression = CreateExpressionTyped<UMaterialExpressionDotProduct>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->A, A, Material,Parameters);;
		SetExpressionInput(Expression->B, B, Material,Parameters);;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionAddConst::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionAdd* Expression = CreateExpressionTyped<UMaterialExpressionAdd>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->A, Input, Material,Parameters);;;
		Expression->ConstB = R;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionClampConst::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionClamp* Expression = CreateExpressionTyped<UMaterialExpressionClamp>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;;
		Expression->MaxDefault = Max;
		Expression->MinDefault = Min;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionDesaturateConst::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionDesaturation* Expression = CreateExpressionTyped<UMaterialExpressionDesaturation>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;;
		{
			UMaterialExpressionConstant* ConstantExpression = CreateExpressionTyped<UMaterialExpressionConstant>(Material);
			ConstantExpression->PreEditChange(nullptr);
			ConstantExpression->R = Percent;
			Expression->Fraction.Expression = ConstantExpression;
			ConstantExpression->PostEditChange();
			ConstantExpression->Modify();
		}
		Expression->Desc = GetLegacyFullName();
		Expression->LuminanceFactors = LuminanceFactors;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionDivideConst::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionDivide* Expression = CreateExpressionTyped<UMaterialExpressionDivide>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->A, Input, Material,Parameters);;;
		Expression->ConstB = R;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionMultiplyConst::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionMultiply* Expression = CreateExpressionTyped<UMaterialExpressionMultiply>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->A, Input, Material,Parameters);;;
		Expression->ConstB = R;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionPlatformSwitch::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (InputPC.Expression)
	{
		return InputPC.Expression->CreateExpression(Material,Parameters);
	}
	return nullptr;
}

UMaterialExpression* ULegacyMaterialExpressionPowerConst::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionPower* Expression = CreateExpressionTyped<UMaterialExpressionPower>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Base, Input, Material,Parameters);;;
		Expression->ConstExponent = Exponent;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionRandomSeed::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionConstant* Expression = CreateExpressionTyped<UMaterialExpressionConstant>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->R = FMath::FRand();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionScreenAspectRatio::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionConstant* Expression = CreateExpressionTyped<UMaterialExpressionConstant>(Material);
		Expression->PreEditChange(nullptr);
		Expression->R = 1.0f;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionShaderCode::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionCustom* Expression = CreateExpressionTyped<UMaterialExpressionCustom>(Material);
		Expression->PreEditChange(nullptr);
		{
			bool I1, I2, I3, I4;
			I1 = ShaderCode.Find(TEXT("%1")) != INDEX_NONE;
			I2 = ShaderCode.Find(TEXT("%2")) != INDEX_NONE;
			I3 = ShaderCode.Find(TEXT("%3")) != INDEX_NONE;
			I4 = ShaderCode.Find(TEXT("%4")) != INDEX_NONE;
			Expression->OutputType = ECustomMaterialOutputType::CMOT_Float3;
			if (ShaderCode.StartsWith(TEXT("float4")))
			{
				Expression->OutputType = ECustomMaterialOutputType::CMOT_Float4;
			}
			else if (ShaderCode.StartsWith(TEXT("float3")))
			{
				Expression->OutputType = ECustomMaterialOutputType::CMOT_Float3;
			}
			else if (ShaderCode.StartsWith(TEXT("float2")))
			{
				Expression->OutputType = ECustomMaterialOutputType::CMOT_Float2;
			}
			else if (ShaderCode.StartsWith(TEXT("float")))
			{
				Expression->OutputType = ECustomMaterialOutputType::CMOT_Float1;
			}
			ShaderCode = ShaderCode.Replace(TEXT("%1"), TEXT("var1"));
			ShaderCode = ShaderCode.Replace(TEXT("%2"), TEXT("var2"));
			ShaderCode = ShaderCode.Replace(TEXT("%3"), TEXT("var3"));
			ShaderCode = ShaderCode.Replace(TEXT("%4"), TEXT("var4"));
			Expression->Code = ShaderCode;
			Expression->Inputs.Empty();
			if (I1)
			{
				Expression->Inputs.AddDefaulted();
				Expression->Inputs.Last().InputName = TEXT("var1");
				SetExpressionInput(Expression->Inputs.Last().Input, A, Material,Parameters);;;
			}
			if (I2)
			{
				Expression->Inputs.AddDefaulted();
				Expression->Inputs.Last().InputName = TEXT("var2");
				SetExpressionInput(Expression->Inputs.Last().Input, B, Material,Parameters);;;
			}
			if (I3)
			{
				Expression->Inputs.AddDefaulted();
				Expression->Inputs.Last().InputName = TEXT("var3");
				SetExpressionInput(Expression->Inputs.Last().Input, C, Material,Parameters);;;
			}
			if (I4)
			{
				Expression->Inputs.AddDefaulted();
				Expression->Inputs.Last().InputName = TEXT("var4");
				SetExpressionInput(Expression->Inputs.Last().Input, D, Material,Parameters);;;
			}
		}
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionSineRange::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		URedUELegacySineRangeMaterialExpression* Expression = CreateExpressionTyped<URedUELegacySineRangeMaterialExpression>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Period  = Period;
		Expression->Min  = Min;
		Expression->Max  = Max;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionSmoothstep::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionSmoothStep* Expression = CreateExpressionTyped<UMaterialExpressionSmoothStep>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Value, X, Material,Parameters);;
		SetExpressionInput(Expression->Min, Min, Material,Parameters);;
		SetExpressionInput(Expression->Max, Max, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionStep::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionStep* Expression = CreateExpressionTyped<UMaterialExpressionStep>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->X, X, Material,Parameters);;
		SetExpressionInput(Expression->Y, Y, Material,Parameters);;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionSubtractConst::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionSubtract* Expression = CreateExpressionTyped<UMaterialExpressionSubtract>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->A, Input, Material,Parameters);;
		Expression->ConstB = R;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionTemp::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (Input.Expression)
	{
		return Input.Expression->CreateExpression(Material,Parameters);
	}
	return nullptr;
}

UMaterialExpression* ULegacyMaterialExpressionTransform2::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTransform* Expression = CreateExpressionTyped<UMaterialExpressionTransform>(Material);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		switch (TransformTypeFrom)
		{
		case ELegacyMaterialVectorCoordTransformFrom::TRANSFORMFROM_World:
			Expression->TransformSourceType = TRANSFORMSOURCE_World;
			break;
		case ELegacyMaterialVectorCoordTransformFrom::TRANSFORMFROM_Local:
			Expression->TransformSourceType = TRANSFORMSOURCE_Local;
			break;
		case ELegacyMaterialVectorCoordTransformFrom::TRANSFORMFROM_Tangent:
			Expression->TransformSourceType = TRANSFORMSOURCE_Tangent;
			break;
		case ELegacyMaterialVectorCoordTransformFrom::TRANSFORMFROM_View:
			Expression->TransformSourceType = TRANSFORMSOURCE_View;
			break;
		}
		switch (TransformTypeTo)
		{
		case ELegacyMaterialVectorCoordTransformTo::TRANSFORMTO_World:
			Expression->TransformType = TRANSFORM_World;
			break;
		case ELegacyMaterialVectorCoordTransformTo::TRANSFORMTO_View:
			Expression->TransformType = TRANSFORM_View;
			break;
		case ELegacyMaterialVectorCoordTransformTo::TRANSFORMTO_Local:
			Expression->TransformType = TRANSFORM_Local;
			break;
		case ELegacyMaterialVectorCoordTransformTo::TRANSFORMTO_Tangent:
			Expression->TransformType = TRANSFORM_Tangent;
			break;
		}
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionTransformPosition2::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTransformPosition* Expression = CreateExpressionTyped<UMaterialExpressionTransformPosition>(Material);
		Expression->Desc = GetLegacyFullName();
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		switch (TransformTypeFrom)
		{
		case ELegacyMaterialPositionTransformFrom::TRANSFORMPOSFROM_World:
			Expression->TransformSourceType = TRANSFORMPOSSOURCE_World;
			break;
		case ELegacyMaterialPositionTransformFrom::TRANSFORMPOSFROM_View:
			Expression->TransformSourceType = TRANSFORMPOSSOURCE_View;
			break;
		case ELegacyMaterialPositionTransformFrom::TRANSFORMPOSFROM_Local:
			Expression->TransformSourceType = TRANSFORMPOSSOURCE_Local;
			break;
		case ELegacyMaterialPositionTransformFrom::TRANSFORMPOSFROM_Tangent:
			Expression->TransformSourceType = TRANSFORMPOSSOURCE_World;
			break;
		}
		switch (TransformTypeTo)
		{
		case ELegacyMaterialPositionTransformTo::TRANSFORMPOSTO_World:
			Expression->TransformType = TRANSFORMPOSSOURCE_World;
			break;
		case ELegacyMaterialPositionTransformTo::TRANSFORMPOSTO_View:
			Expression->TransformType = TRANSFORMPOSSOURCE_View;
			break;
		case ELegacyMaterialPositionTransformTo::TRANSFORMPOSTO_Local:
			Expression->TransformType = TRANSFORMPOSSOURCE_Local;
			break;
		case ELegacyMaterialPositionTransformTo::TRANSFORMPOSTO_Tangent:
			Expression->TransformType = TRANSFORMPOSSOURCE_World;
			break;
		}
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionVS_LocalPosition::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTransformPosition* Expression = CreateExpressionTyped<UMaterialExpressionTransformPosition>(Material);
		Expression->PreEditChange(nullptr);
		{
			UMaterialExpressionWorldPosition* ExpressionWorld = CreateExpressionTyped<UMaterialExpressionWorldPosition>(Material);
			ExpressionWorld->PreEditChange(nullptr);
			Expression->TransformSourceType  = TRANSFORMPOSSOURCE_World;
			Expression->TransformType = TRANSFORMPOSSOURCE_Local;
			Expression->Input.Expression = ExpressionWorld;	
			ExpressionWorld->PostEditChange();
			ExpressionWorld->Modify();
		}
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionVS_TexCoord0::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTextureCoordinate* Expression = CreateExpressionTyped<UMaterialExpressionTextureCoordinate>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->CoordinateIndex = 0;
		Expression->UTiling = 1;
		Expression->VTiling = 1;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionVS_TexCoord1::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionTextureCoordinate* Expression = CreateExpressionTyped<UMaterialExpressionTextureCoordinate>(Material);
		Expression->Desc = GetLegacyFullName();
		Expression->CoordinateIndex = 1;
		Expression->UTiling = 1;
		Expression->VTiling = 1;
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionVS_UndeformedWorldPosition::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionWorldPosition* Expression = CreateExpressionTyped<UMaterialExpressionWorldPosition>(Material);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionRotator::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UClass* MaterialExpressionRotator = LoadClass<UMaterialExpression>(nullptr,TEXT("/Script/Engine.MaterialExpressionRotator"));
		UMaterialExpression* Expression = CreateExpressionTyped<UMaterialExpression>(Material, MaterialExpressionRotator);
		FStructProperty* PropertyCoordinate = FindFProperty<FStructProperty>(MaterialExpressionRotator, "Coordinate");
		FStructProperty* PropertyTime = FindFProperty<FStructProperty>(MaterialExpressionRotator, "Time");
		FFloatProperty* PropertySpeed = FindFProperty<FFloatProperty>(MaterialExpressionRotator, "Speed");
		FFloatProperty* PropertyCenterX = FindFProperty<FFloatProperty>(MaterialExpressionRotator, "CenterX");
		FFloatProperty* PropertyCenterY = FindFProperty<FFloatProperty>(MaterialExpressionRotator, "CenterY");
		Expression->PreEditChange(nullptr);
		 SetExpressionInput(*PropertyTime->ContainerPtrToValuePtr<FExpressionInput>(Expression), Time, Material,Parameters);;
		 SetExpressionInput(*PropertyCoordinate->ContainerPtrToValuePtr<FExpressionInput>(Expression), Coordinate, Material,Parameters);;
		 *PropertySpeed->ContainerPtrToValuePtr<float>(Expression)=Speed;
		 *PropertyCenterX->ContainerPtrToValuePtr<float>(Expression)=CenterX;
		 *PropertyCenterY->ContainerPtrToValuePtr<float>(Expression)=CenterY;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}

UMaterialExpression* ULegacyMaterialExpressionSine::CreateExpression(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters)
{
	if (!CurrentExpression)
	{
		UMaterialExpressionSine* Expression = CreateExpressionTyped<UMaterialExpressionSine>(Material);
		Expression->PreEditChange(nullptr);
		SetExpressionInput(Expression->Input, Input, Material,Parameters);;
		Expression->Period = Period;
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
	
}

UMaterialExpression* ULegacyMaterialExpressionCameraWorldPosition::CreateExpression(UMaterial* Material, const FLegacyMaterialExpressionParameters& Parameters)
{
	if (!CurrentExpression)
	{
		UClass* MaterialExpressionCameraPositionWS = LoadClass<UMaterialExpression>(nullptr,TEXT("/Script/Engine.MaterialExpressionCameraPositionWS"));
		UMaterialExpression* Expression = CreateExpressionTyped<UMaterialExpression>(Material, MaterialExpressionCameraPositionWS);
		Expression->PreEditChange(nullptr);
		Expression->Desc = GetLegacyFullName();
		Expression->PostEditChange();
		Expression->Modify();
		CurrentExpression = Expression;
	}
	return CurrentExpression;
}
