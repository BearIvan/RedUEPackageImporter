#pragma once

#include "Core/LegacyObject.h"
#include "LegacyMaterialExpression.generated.h"

USTRUCT(BlueprintType)
struct FLegacyExpressionInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	class ULegacyMaterialExpression* Expression;
	
	UPROPERTY(BlueprintReadWrite)
	int32	Mask;
	
	UPROPERTY(BlueprintReadWrite)
	int32	MaskR;
	
	UPROPERTY(BlueprintReadWrite)
	int32	MaskG;
	
	UPROPERTY(BlueprintReadWrite)
	int32	MaskB;
	
	UPROPERTY(BlueprintReadWrite)
	int32	MaskA;
};

struct FLegacyMaterialExpressionParameters
{
	bool 														IsTextureDeformer = false;
	bool 														WorldDeformer = false;
	FGuid														CustomVertexColorGuid;
	TObjectPtr<class UMaterialExpressionNamedRerouteDeclaration>CustomVertexColor;
	bool 														VertexColor = false;
};

UCLASS()
class REDUELEGACY_API ULegacyMaterialExpression : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual void					LegacySerialize						(FRedUELegacyArchive& Ar) override;
	virtual bool					LegacySupport_Implementation		(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual UMaterialExpression*	CreateExpression					(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters);
	virtual	void					FixExpressionInput					(FExpressionInput& Out);
			void					SetExpressionInput					(FExpressionInput&Out,const FLegacyExpressionInput&In,UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters);
			UMaterialExpression*	StartCreateExpression				(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters);
			static void				Export								(UMaterial* OutMaterial,class ULegacyMaterial3*InMaterial);
	UPROPERTY(Transient)
	UMaterialExpression*			CurrentExpression;
};

UCLASS()
class REDUELEGACY_API ULegacyMaterialExpressionTextureBase : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	class ULegacyTexture3* Texture;
};

UCLASS()
class REDUELEGACY_API ULegacyMaterialExpressionTextureSample : public ULegacyMaterialExpressionTextureBase
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	virtual	void					FixExpressionInput	(FExpressionInput& Out);
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Coordinates;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput SamplerSource;

	
};

UCLASS()
class ULegacyMaterialExpressionTextureSampleParameter : public ULegacyMaterialExpressionTextureSample
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FName ParameterName;
	
	UPROPERTY(BlueprintReadWrite)
	FName Group;
};

UCLASS()
class ULegacyMaterialExpressionTextureSampleParameter2D : public ULegacyMaterialExpressionTextureSampleParameter
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionTextureSampleParameterCube : public ULegacyMaterialExpressionTextureSampleParameter
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionParameter : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FName ParameterName;
	
	UPROPERTY(BlueprintReadWrite)
	FName Group;
};

UCLASS()
class ULegacyMaterialExpressionScalarParameter : public ULegacyMaterialExpressionParameter
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	float DefaultValue;
};

UCLASS()
class ULegacyMaterialExpressionStaticBoolParameter : public ULegacyMaterialExpressionParameter
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	bool DefaultValue;
};

UCLASS()
class ULegacyMaterialExpressionStaticSwitchParameter : public ULegacyMaterialExpressionStaticBoolParameter
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput A;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput B;
};

UCLASS()
class ULegacyMaterialExpressionVectorParameter : public ULegacyMaterialExpressionParameter
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor DefaultValue;
};
UCLASS()
class ULegacyMaterialExpressionConstant : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	float R;
	
};

UCLASS()
class ULegacyMaterialExpressionConstant2Vector : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	float R;
	
	UPROPERTY(BlueprintReadWrite)
	float G;
	
};

UCLASS()
class ULegacyMaterialExpressionConstant3Vector : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	float R;
	
	UPROPERTY(BlueprintReadWrite)
	float G;
	
	UPROPERTY(BlueprintReadWrite)
	float B;
	
};

UCLASS()
class ULegacyMaterialExpressionConstant4Vector : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	float R;
	
	UPROPERTY(BlueprintReadWrite)
	float G;
	
	UPROPERTY(BlueprintReadWrite)
	float B;
	
	UPROPERTY(BlueprintReadWrite)
	float A;
	
};

UCLASS()
class ULegacyMaterialExpressionTextureCoordinate : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int32 CoordinateIndex;
	
	UPROPERTY(BlueprintReadWrite)
	float UTiling = 1;
	
	UPROPERTY(BlueprintReadWrite)
	float VTiling = 1;
	
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;

};

UENUM()
enum class ELegacyMaterialVectorCoordTransformSource
{
	TRANSFORMSOURCE_World,
	TRANSFORMSOURCE_Local,
	TRANSFORMSOURCE_Tangent,
	TRANSFORMSOURCE_View,
};

UENUM()
enum class ELegacyMaterialVectorCoordTransform
{
	TRANSFORM_World,
	TRANSFORM_View,
	TRANSFORM_Local,
	TRANSFORM_Tangent,
};

UCLASS()
class ULegacyMaterialExpressionTransform : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyMaterialVectorCoordTransformSource TransformSourceType;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyMaterialVectorCoordTransform TransformType;
};

UCLASS()
class ULegacyMaterialExpressionReflectionVector : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UENUM()
enum  class ELegacyMaterialPositionTransformSource
{
	TRANSFORMPOSSOURCE_Local,
	TRANSFORMPOSSOURCE_PostProjection
};

UENUM()
enum class ELegacyMaterialPositionTransform
{
	TRANSFORMPOS_World
};

UCLASS()
class ULegacyMaterialExpressionTransformPosition : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyMaterialPositionTransformSource TransformSourceType;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyMaterialPositionTransform TransformType;
};

UCLASS()
class ULegacyMaterialExpressionAdd : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput A;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput B;

};

UCLASS()
class ULegacyMaterialExpressionSubtract : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput A;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput B;
};

UCLASS()
class ULegacyMaterialExpressionMultiply : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput A;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput B;
};

UCLASS()
class ULegacyMaterialExpressionDivide : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput A;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput B;
};

UCLASS()
class ULegacyMaterialExpressionPower : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Base;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Exponent;
};

UCLASS()
class ULegacyMaterialExpressionClamp : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Min;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Max;
};

UCLASS()
class ULegacyMaterialExpressionTime : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIgnorePause ;
};

UCLASS()
class ULegacyMaterialExpressionOneMinus : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
};

UCLASS()
class ULegacyMaterialExpressionFrac : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
};

UCLASS()
class ULegacyMaterialExpressionLinearInterpolate : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput A;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput B;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Alpha;
};

UCLASS()
class ULegacyMaterialExpressionLightVector : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
};

UCLASS()
class ULegacyMaterialExpressionComponentMask : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	bool R;
	
	UPROPERTY(BlueprintReadWrite)
	bool G;
	
	UPROPERTY(BlueprintReadWrite)
	bool B;
	
	UPROPERTY(BlueprintReadWrite)
	bool A;

};

UCLASS()
class ULegacyMaterialExpressionAbs : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;

};

UCLASS()
class ULegacyMaterialExpressionMeshEmitterVertexColor : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionPixelDepth : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionDestDepth : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionAppendVector : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput A;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput B;
};

UCLASS()
class ULegacyMaterialExpressionCameraVector : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionPanner : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Coordinate;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Time;
	
	UPROPERTY(BlueprintReadWrite)
	float SpeedX;
	
	UPROPERTY(BlueprintReadWrite)
	float SpeedY;
};

UCLASS()
class ULegacyMaterialExpressionConstantClamp : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	float Min;
	
	UPROPERTY(BlueprintReadWrite)
	float Max=1;
};

UCLASS()
class ULegacyMaterialExpressionWorldPosition : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionVertexColor : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionFresnel : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	float Exponent;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Normal;
};

UCLASS()
class ULegacyMaterialExpressionFloor : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
};

UCLASS()
class ULegacyMaterialExpressionStaticComponentMaskParameter : public ULegacyMaterialExpressionParameter
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	bool DefaultR;
	
	UPROPERTY(BlueprintReadWrite)
	bool DefaultG;
	
	UPROPERTY(BlueprintReadWrite)
	bool DefaultB;
	
	UPROPERTY(BlueprintReadWrite)
	bool DefaultA;
	

};

UCLASS()
class ULegacyMaterialExpressionDesaturation : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Percent;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor LuminanceFactors = { 0.3,0.59,0.11 };
};

UCLASS()
class ULegacyMaterialExpressionBumpOffset : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Coordinate;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Height;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput HeightRatioInput;
	
	UPROPERTY(BlueprintReadWrite)
	float HeightRatio = 0.05f;
	
	UPROPERTY(BlueprintReadWrite)
	float ReferencePlane = 0.5f;

};

UCLASS()
class ULegacyMaterialExpressionNormalize : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput VectorInput;
};

UCLASS()
class ULegacyMaterialExpressionDotProduct : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput A;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput B;
};

UCLASS()
class ULegacyMaterialExpressionAddConst : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	float R;
	
};

UCLASS()
class ULegacyMaterialExpressionClampConst : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	float Min;
	
	UPROPERTY(BlueprintReadWrite)
	float Max=1;
};

UCLASS()
class ULegacyMaterialExpressionDesaturateConst : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	float Percent;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor LuminanceFactors;
};

UCLASS()
class ULegacyMaterialExpressionDivideConst : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	float R=1.f;
};


UCLASS()
class ULegacyMaterialExpressionMultiplyConst : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	float R = 1.f;
};

UCLASS()
class ULegacyMaterialExpressionPlatformSwitch : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput InputPC;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput InputXBOX;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput InputPS3;
};

UCLASS()
class ULegacyMaterialExpressionPowerConst : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	float Exponent;
};

UCLASS()
class ULegacyMaterialExpressionRandomSeed : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionScreenAspectRatio : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;

};

UCLASS()
class ULegacyMaterialExpressionShaderCode : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput A;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput B;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput C;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput D;
	
	UPROPERTY(BlueprintReadWrite)
	FString ShaderCode;
};

UCLASS()
class ULegacyMaterialExpressionSineRange : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	float Period=1;
	
	UPROPERTY(BlueprintReadWrite)
	float Min;
	
	UPROPERTY(BlueprintReadWrite)
	float Max=1;
};

UCLASS()
class ULegacyMaterialExpressionSmoothstep : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Min;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Max;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput X;
};

UCLASS()
class ULegacyMaterialExpressionStep : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput X;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Y;
};

UCLASS()
class ULegacyMaterialExpressionSubtractConst : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	float R;
};

UCLASS()
class ULegacyMaterialExpressionTemp : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
};

UENUM()
enum class ELegacyMaterialVectorCoordTransformFrom
{
	TRANSFORMFROM_World,
	TRANSFORMFROM_Tangent,
	TRANSFORMFROM_View,
	TRANSFORMFROM_Local,
	TRANSFORMFROM_MAX
};
UENUM()
enum class ELegacyMaterialVectorCoordTransformTo
{
	TRANSFORMTO_World,
	TRANSFORMTO_Tangent,
	TRANSFORMTO_View,
	TRANSFORMTO_Local,
	TRANSFORMTO_MAX
};

UCLASS()
class ULegacyMaterialExpressionTransform2 : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyMaterialVectorCoordTransformFrom TransformTypeFrom;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyMaterialVectorCoordTransformTo TransformTypeTo;
};
UENUM()
enum class ELegacyMaterialPositionTransformFrom
{
	TRANSFORMPOSFROM_World,
	TRANSFORMPOSFROM_View,
	TRANSFORMPOSFROM_Local,
	TRANSFORMPOSFROM_Tangent,
	TRANSFORMPOSFROM_MAX
};
UENUM()
enum class ELegacyMaterialPositionTransformTo
{
	TRANSFORMPOSTO_World,
	TRANSFORMPOSTO_View,
	TRANSFORMPOSTO_Local,
	TRANSFORMPOSTO_Tangent,
	TRANSFORMPOSTO_MAX
};

UCLASS()
class ULegacyMaterialExpressionTransformPosition2 : public ULegacyMaterialExpression
{

	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyMaterialPositionTransformFrom TransformTypeFrom;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyMaterialPositionTransformTo TransformTypeTo;
};

UCLASS()
class ULegacyMaterialExpressionVS_LocalPosition : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionVS_TexCoord0 : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionVS_TexCoord1 : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionVS_UndeformedWorldPosition : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};


UCLASS()
class ULegacyMaterialExpressionRotator : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Coordinate;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Time;
	
	UPROPERTY(BlueprintReadWrite)
	float CenterX;
	
	UPROPERTY(BlueprintReadWrite)
	float CenterY;
	
	UPROPERTY(BlueprintReadWrite)
	float Speed;
	
};
UCLASS()
class ULegacyMaterialExpressionSine : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Input;
	
	UPROPERTY(BlueprintReadWrite)
	float Period;
	
};

UCLASS()
class ULegacyMaterialExpressionCameraWorldPosition : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionScreenPosition : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
};

UCLASS()
class ULegacyMaterialExpressionDepthBiasedAlpha  : public ULegacyMaterialExpression
{
	GENERATED_BODY()
public:	
	virtual UMaterialExpression*	CreateExpression	(UMaterial* Material,const FLegacyMaterialExpressionParameters&Parameters) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyExpressionInput Alpha;
};