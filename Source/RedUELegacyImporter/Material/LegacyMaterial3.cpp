#include "Material/LegacyMaterial3.h"

#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/RedUELegacyArchive.h"

void ULegacyMaterial3::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	if (Ar.Game == ERedUELegacyGame::Bioshock3)
	{
		FGuid Guid;
		Ar<<Guid;
		for (int32 k = 0; k < 5; ++k )
		{
			TArray<ULegacyObject*> PlatformUniformExpressionTextures;
			Ar<<PlatformUniformExpressionTextures;
			for (ULegacyObject*Texture:PlatformUniformExpressionTextures)
			{
				if (Texture)
				{
					ReferencedTextures.Add(Texture);
				}
			}
		}
	}
	Ar.Seek(Ar.GetStopper());
}

UObject* ULegacyMaterial3::ExportToContent()
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

	const FString ObjectPath = GetOutContentPath()/ GetLegacyFullName().Replace(TEXT("."),TEXT("/"));
	const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
	const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
	UMaterialInterface* MaterialResult = LoadObject<UMaterialInterface>(nullptr, *FullObjectPath,nullptr,LOAD_NoWarn);
	if(!MaterialResult)
	{
	
		
		UPackage*  AssetPackage = CreatePackage(*PackageName);
		UMaterial* Material = NewObject<UMaterial>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public|RF_Standalone);
		Material->PreEditChange(nullptr);
		ULegacyMaterialExpression::Export(Material,this);
	
		
		switch (BlendMode)
		{
		case ELegacyBlendMode::BLEND_Opaque:
			Material->BlendMode = BLEND_Opaque;
			break;
		case ELegacyBlendMode::BLEND_Masked:
			Material->BlendMode = BLEND_Masked;
			break;
		case ELegacyBlendMode::BLEND_Translucent:
			Material->BlendMode = BLEND_Translucent;
			break;
		case ELegacyBlendMode::BLEND_Additive:
			Material->BlendMode = BLEND_Additive;
			break;
		case ELegacyBlendMode::BLEND_Modulate:
			Material->BlendMode = BLEND_Modulate;
			break;
		case ELegacyBlendMode::BLEND_ModulateAndAdd:
			Material->BlendMode = BLEND_Modulate;
			break;
		case ELegacyBlendMode::BLEND_SoftMasked:
			Material->BlendMode = BLEND_Masked;
			break;
		case ELegacyBlendMode::BLEND_AlphaComposite:
			Material->BlendMode = BLEND_AlphaComposite;
			break;
		case ELegacyBlendMode::BLEND_DitheredTranslucent:
			Material->BlendMode = BLEND_Translucent;
			break;
		default: ;
		}
		{
			float Y = Material->EditorY;
			float X = Material->EditorX-100;
			TSet<class UMaterialExpression*> Nodes;
			SortNode(Material->GetEditorOnlyData()->BaseColor.Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->EmissiveColor.Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->Normal.Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->Opacity.Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->OpacityMask.Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->Specular.Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->Roughness.Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->WorldPositionOffset.Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->CustomizedUVs[0].Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->CustomizedUVs[1].Expression, Nodes, Y, X);
			SortNode(Material->GetEditorOnlyData()->MaterialAttributes.Expression, Nodes, Y, X);
		}
		switch (LightingModel)
		{
		case ELegacyMaterialLightingModel::MLM_Phong:
			Material->SetShadingModel(MSM_DefaultLit);
			break;
		case ELegacyMaterialLightingModel::MLM_NonDirectional:
			Material->SetShadingModel(MSM_DefaultLit);
			break;
		case ELegacyMaterialLightingModel::MLM_Unlit:
		case ELegacyMaterialLightingModel::MLM_Custom:
			Material->SetShadingModel(MSM_Unlit);
			break;
		case ELegacyMaterialLightingModel::MLM_SHPRT:
		case ELegacyMaterialLightingModel::MLM_Anisotropic:
		default:
			ensureMsgf(false,TEXT("Unkown Shading Mode"));
			break;
		}
		Material->TwoSided = TwoSided;
		FAssetRegistryModule::AssetCreated(Material);
		Material->PostEditChange();
		Material->Modify();
		MaterialResult = Material;
	}
	PresentObject = MaterialResult;
	return MaterialResult;
}

FName ULegacyMaterial3::GetLegacyClassName_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	static FName NAME_Material = "Material";
	return NAME_Material;
}

bool ULegacyMaterial3::LegacySupport_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	if(EngineType != ERedUELegacyEngineType::UnrealEngine3)
	{
		return false;
	}
	return Super::LegacySupport_Implementation(EngineType, GameType);
}

bool ULegacyMaterial3::SortNode(UMaterialExpression* Start, TSet<UMaterialExpression*>& Nodes, float& Y, float X)
{
	if (!Start)return false;
	if (Nodes.Contains(Start))return false;
	Nodes.Add(Start);
	Start->MaterialExpressionEditorX = X;
	Start->MaterialExpressionEditorY = Y;
	TArrayView<FExpressionInput*> Inputs = Start->GetInputsView();
	for (int i = 0; i <Inputs.Num(); i++)
	{
		if(SortNode(Inputs[i]->Expression, Nodes, Y, X - 500))
			Y += 150;
	}
	return true;
}
