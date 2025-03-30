#pragma once

#include "LegacyMaterialExpression.h"
#include "LegacyMaterialInterface.h"
#include "LegacyMaterial3.generated.h"

UENUM()
enum class ELegacyBlendMode
{
	BLEND_Opaque,
	BLEND_Masked,
	BLEND_Translucent,
	BLEND_Additive,
	BLEND_Modulate,
	BLEND_ModulateAndAdd,
	BLEND_SoftMasked,
	BLEND_AlphaComposite,
	BLEND_DitheredTranslucent
};

UENUM()
enum class ELegacyMaterialLightingModel
{
	MLM_Phong               =0,
	MLM_NonDirectional      =1,
	MLM_Unlit               =2,
	MLM_SHPRT               =3,
	MLM_Custom              =4,
	MLM_Anisotropic         =5,
};

USTRUCT(BlueprintType)
struct FLegacyMaterialInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ULegacyMaterialExpression* Expression;
	
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

USTRUCT(BlueprintType)
struct FLegacyColorMaterialInput :public FLegacyMaterialInput
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FLegacyScalarMaterialInput :public FLegacyMaterialInput
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FLegacyVectorMaterialInput :public FLegacyMaterialInput
{
	GENERATED_BODY()
};
USTRUCT(BlueprintType)
struct FLegacyVector2MaterialInput :public FLegacyMaterialInput
{
	GENERATED_BODY()
};

UCLASS()
class REDUELEGACY_API ULegacyMaterial3 : public ULegacyMaterialInterface
{
	GENERATED_BODY()

public:
	virtual void		LegacySerialize						(FRedUELegacyArchive& Ar) override;
	virtual UObject*	ExportToContent						() override;
	virtual FName		GetLegacyClassName_Implementation	(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual bool		LegacySupport_Implementation		(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
			bool		SortNode							(class UMaterialExpression* Start, TSet<class UMaterialExpression*>& Nodes, float& Y, float X);

	UPROPERTY(BlueprintReadWrite)
	FLegacyColorMaterialInput DiffuseColor;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyScalarMaterialInput DiffusePower;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyColorMaterialInput SpecularColor;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyScalarMaterialInput SpecularPower;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyVectorMaterialInput Normal;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyColorMaterialInput EmissiveColor;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyScalarMaterialInput Opacity;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyScalarMaterialInput OpacityMask;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyVector2MaterialInput TextureDeformer0;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyVector2MaterialInput TextureDeformer1;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyColorMaterialInput WorldDeformer;

	UPROPERTY(BlueprintReadWrite)
	FLegacyVectorMaterialInput LocalDeformer;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyVectorMaterialInput LocalTangentDeformer;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyVectorMaterialInput WorldTangentDeformer;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyColorMaterialInput VertexColorDeformer;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyScalarMaterialInput VertexOpacityDeformer;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyScalarMaterialInput GbufferUserValue;
	
	
	UPROPERTY(BlueprintReadWrite)
	bool TwoSided;
	
	UPROPERTY(BlueprintReadWrite)
	bool bDisableDepthTest;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsMasked;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyBlendMode BlendMode;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyMaterialLightingModel LightingModel;
	
	UPROPERTY(BlueprintReadWrite)
	float OpacityMaskClipValue;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyObject*> ReferencedTextures;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyMaterialExpression*> Expressions;
};
