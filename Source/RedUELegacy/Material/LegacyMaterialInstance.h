#pragma once

#include "LegacyMaterialInterface.h"
#include "LegacyMaterialInstance.generated.h"

USTRUCT(BlueprintType)
struct FLegacyScalarParameterValue
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FName ParameterName;
	
	UPROPERTY(BlueprintReadWrite)
	float ParameterValue;
};

USTRUCT(BlueprintType)
struct FLegacyTextureParameterValue
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FName ParameterName;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyTexture3* ParameterValue;
};

USTRUCT(BlueprintType)
struct FLegacyVectorParameterValue
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FName ParameterName;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor ParameterValue;
};

USTRUCT(BlueprintType)
struct FLegacyStaticParameterBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	bool bOverride;
};

USTRUCT(BlueprintType)
struct FLegacyStaticSwitchParameter : public FLegacyStaticParameterBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FName ParameterName;
	
	UPROPERTY(BlueprintReadWrite)
	bool Value;
	
	UPROPERTY(BlueprintReadWrite)
	FGuid ExpressionGUID;
	friend FArchive& operator<<(FArchive& Ar, FLegacyStaticSwitchParameter& P)
	{
		Ar << P.ParameterName << P.Value << P.bOverride;
		Ar << P.ExpressionGUID;
		return Ar;
	}
};

USTRUCT(BlueprintType)
struct FLegacyStaticComponentMaskParameter : public FLegacyStaticParameterBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FName ParameterName;
	
	UPROPERTY(BlueprintReadWrite)
	bool R;
	
	UPROPERTY(BlueprintReadWrite)
	bool G;
	
	UPROPERTY(BlueprintReadWrite)
	bool B;
	
	UPROPERTY(BlueprintReadWrite)
	bool A;
	
	UPROPERTY(BlueprintReadWrite)
	FGuid ExpressionGUID;
	friend FArchive& operator<<(FArchive& Ar, FLegacyStaticComponentMaskParameter& P)
	{
		Ar << P.ParameterName << P.R << P.G << P.B << P.A << P.bOverride;
		Ar << P.ExpressionGUID;
		return Ar;
	}
};
USTRUCT(BlueprintType)
struct FLegacyStaticParameterSet
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FGuid BaseMaterialId;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyStaticSwitchParameter> StaticSwitchParameters;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyStaticComponentMaskParameter> StaticComponentMaskParameters;

};

UCLASS()
class REDUELEGACY_API ULegacyMaterialInstance : public ULegacyMaterialInterface
{
	GENERATED_BODY()
public:
	virtual bool		LegacySupport_Implementation		(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual void        LegacySerialize                     (FRedUELegacyArchive& Ar) override;
	virtual UObject*	ExportToContent						() override;
			void		ReadUniformExpressions				(FArchive& Ar);
	UPROPERTY(BlueprintReadWrite)
	ULegacyMaterialInterface* Parent;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyObject*> ReferencedTextures;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyScalarParameterValue>	ScalarParameterValues;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyTextureParameterValue> TextureParameterValues;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyVectorParameterValue>	VectorParameterValues;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyStaticParameterSet	StaticParameters;

	UPROPERTY(BlueprintReadWrite)
	bool	bHasStaticPermutationResource;
};

UCLASS()
class REDUELEGACY_API ULegacyMaterialInstanceConstant: public ULegacyMaterialInstance
{
	GENERATED_BODY()
};