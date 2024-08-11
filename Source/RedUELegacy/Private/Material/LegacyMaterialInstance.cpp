#include "Material/LegacyMaterialInstance.h"

#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacyGame.h"
#include "Material/LegacyTexture3.h"

bool ULegacyMaterialInstance::LegacySupport_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	if(EngineType != ERedUELegacyEngineType::UnrealEngine3)
	{
		return false;
	}
	return Super::LegacySupport_Implementation(EngineType, GameType);
}

void ULegacyMaterialInstance::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	if (Ar.Game == ERedUELegacyGame::Singularity)
	{
		if (bHasStaticPermutationResource)
		{
			TArray<FString> CompileErrors;
			Ar<< CompileErrors;
			TMap<UObject*, int> DummyMap;
			Ar << DummyMap;
			int MaxTextureDependencyLength;
			Ar << MaxTextureDependencyLength;
			FGuid Id;
			Ar << Id;
			int NumUserTexCoords;
			Ar << NumUserTexCoords;
			{
				int Count;
				Ar<<Count;
				for (int i = 0; i < Count; i++)
				{
					ReadUniformExpressions(Ar);
				}
			}
			{
				int Count;
				Ar << Count;
				for (int i = 0; i < Count; i++)
				{
					ReadUniformExpressions(Ar);
				}
			}
			{
				int Count;
				Ar << Count;
				for (int i = 0; i < Count; i++)
				{
					ReadUniformExpressions(Ar);
				}
			}
			{
				int Count;
				Ar << Count;
				for (int i = 0; i < Count; i++)
				{
					ReadUniformExpressions(Ar);
				}

			}
			int bUsesSceneColorTemp ;
			Ar << bUsesSceneColorTemp;

			int bUsesSceneDepthTemp;
			Ar << bUsesSceneDepthTemp;

			int bUsesDynamicParameterTemp;
			Ar << bUsesDynamicParameterTemp;

			int UsingTransforms;
			Ar << UsingTransforms;

			int Unkown1;
			Ar << Unkown1;
			Ar << Unkown1;
			Ar << Unkown1;
			Ar << Unkown1;

			{
				int Count;
				Ar << Count;
				for (int i = 0; i < Count; i++)
				{
					int Unkown2;
					Ar << Unkown2;
					Ar << Unkown2;
					Ar << Unkown2;
					Ar << Unkown2;
				}

			}
			Ar.Seek(10*4+Ar.Tell());
			Ar << StaticParameters.BaseMaterialId << StaticParameters.StaticSwitchParameters << StaticParameters.StaticComponentMaskParameters;
		}
	}
	Ar.Seek(Ar.GetStopper());
}

UObject* ULegacyMaterialInstance::ExportToContent()
{
	if(PresentObject)
	{
		return PresentObject;
	}
	for(ULegacyObject*Texture:ReferencedTextures)
	{
		if(Texture)
		{
			Texture->ExportToContent();
		};
	}
	if(!Parent)
	{
		return nullptr;
	}
	const FString ObjectPath = GetOutContentPath()/ GetLegacyFullName().Replace(TEXT("."),TEXT("/"));
	const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
	const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
	UMaterialInterface* MaterialResult = LoadObject<UMaterialInterface>(nullptr, *FullObjectPath,nullptr,LOAD_NoWarn);
	if(!MaterialResult)
	{
		UPackage*  AssetPackage = CreatePackage(*PackageName);
		UMaterialInstanceConstant* Material = NewObject<UMaterialInstanceConstant>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public|RF_Standalone);
		Material->PreEditChange(nullptr);
		FAssetRegistryModule::AssetCreated(Material);
		FStaticParameterSet NewStaticParameterSet;

		for(const FLegacyStaticSwitchParameter&StaticSwitchParameter:StaticParameters.StaticSwitchParameters)
		{
			FStaticSwitchParameter SwitchParameter;
			SwitchParameter.ParameterInfo.Name = StaticSwitchParameter.ParameterName;
			SwitchParameter.Value = StaticSwitchParameter.Value;
			SwitchParameter.bOverride = StaticSwitchParameter.bOverride;
			NewStaticParameterSet.StaticSwitchParameters.Add(SwitchParameter);
		}
		
		for(const FLegacyStaticComponentMaskParameter&StaticComponentMaskParameter:StaticParameters.StaticComponentMaskParameters)
		{
			FStaticComponentMaskParameter SwitchParameter;
			SwitchParameter.ParameterInfo.Name = StaticComponentMaskParameter.ParameterName;
			SwitchParameter.R = StaticComponentMaskParameter.R;
			SwitchParameter.G = StaticComponentMaskParameter.G;
			SwitchParameter.B = StaticComponentMaskParameter.B;
			SwitchParameter.A = StaticComponentMaskParameter.A;
			SwitchParameter.bOverride = StaticComponentMaskParameter.bOverride;
			NewStaticParameterSet.EditorOnly.StaticComponentMaskParameters.Add(SwitchParameter);
		}
		for(const FLegacyScalarParameterValue&ScalarParameterValue:ScalarParameterValues)
		{
			Material->SetScalarParameterValueEditorOnly(FMaterialParameterInfo(ScalarParameterValue.ParameterName), ScalarParameterValue.ParameterValue);
		}
		for(const FLegacyVectorParameterValue&VectorParameterValue:VectorParameterValues)
		{
			Material->SetVectorParameterValueEditorOnly(FMaterialParameterInfo(VectorParameterValue.ParameterName), VectorParameterValue.ParameterValue);
		}
		for(const FLegacyTextureParameterValue&TextureParameterValue:TextureParameterValues)
		{
			if(TextureParameterValue.ParameterValue)
			{
				if(UTexture* Texture = CastChecked<UTexture>(TextureParameterValue.ParameterValue->ExportToContent(),ECastCheckedType::NullAllowed))
				{
					Material->SetTextureParameterValueEditorOnly(FMaterialParameterInfo(TextureParameterValue.ParameterName), Texture);
				}
			}
			
		}
		
		Material->Parent = CastChecked<UMaterialInterface>( Parent->ExportToContent());
		Material->UpdateStaticPermutation(NewStaticParameterSet);
		Material->InitStaticPermutation();
		Material->PostEditChange();
		Material->Modify();
		MaterialResult = Material;
	}
	PresentObject = MaterialResult;
	return MaterialResult;
}

void ULegacyMaterialInstance::ReadUniformExpressions(FArchive& Ar)
{
	
	FName Type;
	Ar << Type;
	if (Type == "FMaterialUniformExpressionMin")
	{
		ReadUniformExpressions(Ar);
		ReadUniformExpressions(Ar);
	}
	else if(Type == "FMaterialUniformExpressionMax")
	{
		ReadUniformExpressions(Ar);
		ReadUniformExpressions(Ar);
	}
	else if (Type == "FMaterialUniformExpressionClamp")
	{
		ReadUniformExpressions(Ar);
		ReadUniformExpressions(Ar);
		ReadUniformExpressions(Ar);
	}
	else if(Type == "FMaterialUniformExpressionSmoothstep")
	{
		ReadUniformExpressions(Ar);
		ReadUniformExpressions(Ar);
		ReadUniformExpressions(Ar);
	}
	else if (Type == "FMaterialUniformExpressionPeriodic")
	{
		ReadUniformExpressions(Ar);
	}
	else if (Type == "FMaterialUniformExpressionStep")
	{
		ReadUniformExpressions(Ar);
		ReadUniformExpressions(Ar);
	}
	else if (Type == "FMaterialUniformExpressionSine")
	{
		ReadUniformExpressions(Ar);
		bool temp;
		Ar<< temp;
	}
	else if(Type == "FMaterialUniformExpressionFrac")
	{
		ReadUniformExpressions(Ar);
	}
	else if(Type == "FMaterialUniformExpressionConstant")
	{
		float Data;
		Ar << Data;
		Ar << Data;
		Ar << Data;
		Ar << Data;
		char Type1;
		Ar << Type1;
	}
	else if (Type == "FMaterialUniformExpressionGameVector")
	{
		int Data;
		Ar << Data;
	}
	else if (Type == "FMaterialUniformExpressionAppendVector")
	{
		ReadUniformExpressions(Ar);
		ReadUniformExpressions(Ar);
		int Data;
		Ar << Data;
	}
	else if (Type == "FMaterialUniformExpressionVectorParameter")
	{
		FName Name;
		Ar << Name;
		float Data;
		Ar << Data;
		Ar << Data;
		Ar << Data;
		Ar << Data;

	}
	else if (Type == "FMaterialUniformExpressionScalarParameter")
	{
		FName Name;
		Ar << Name;
		float Data;
		Ar << Data;
	}
	else if (Type == "FMaterialUniformExpressionTime")
	{
	}
	else if (Type == "FMaterialUniformExpressionTexture")
	{
		UObject* Object;
		Ar << Object;
	}
	else if (Type == "FMaterialUniformExpressionTextureParameter")
	{
		FName Name;
		Ar << Name;
		UObject* Object;
		Ar << Object;
	}
	else if (Type == "FMaterialUniformExpressionTexture")
	{
		UObject* Object;
		Ar << Object;
	}
	else if (Type == "FMaterialUniformExpressionFoldedMath")
	{
		ReadUniformExpressions(Ar);
		ReadUniformExpressions(Ar);
		char Op;
		Ar<<Op;
	}
	else
	{
		checkNoEntry();
	}
	int Data;
	Ar << Data;
	Ar << Data;
}
