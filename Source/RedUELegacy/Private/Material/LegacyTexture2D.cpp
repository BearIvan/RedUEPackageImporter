#include "Material/LegacyTexture2D.h"

#include "Editor.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "RedImage.hpp"
#include "SingularityTextureFileCache.h"
#include "Core/RedUELegacySubsystem.h"

void FLegacyTexture2DMipMap::Serialize(FRedUELegacyArchive& Ar)
{
	Data.Serialize(Ar);
	Ar << SizeX << SizeY;
}

void ULegacyTexture2D::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	if (Ar.LegacyVer < 297)
	{
		int Format2;
		Ar << SizeX << SizeY << Format2;
		Format = static_cast<ELegacyPixelFormat>(Format2);		// int -> byte (enum)
	}
	Ar<<Mips;
	
	if (Ar.Game ==  ERedUELegacyGame::Bioshock3) return;
	if (Ar.LegacyVer >= 567)
	{
		Ar << TextureFileCacheGuid;
	}
	Ar.Seek(Ar.GetStopper());
	if (Ar.Game == ERedUELegacyGame::Singularity)
	{
		URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
		if (FSingularityTextureFileCacheInfo* Info = RedUELegacySubsystem->SingularityTextureFileCache->Get(TextureFileCacheGuid))
		{
			TextureFileCacheName = "coalesced";
			for (size_t i = 0; i < Mips.Num() && i < 6; i++)
			{
				if (Mips[i].Data.CanReloadBulk() && !(Mips[i].Data.BulkDataFlags& BULKDATA_Unused))
				{
		
					if (Info)
					{
						Mips[i].Data.BulkDataOffsetInFile = Info->Mips[i].OffsetInFile;
						Mips[i].Data.BulkDataSizeOnDisk = Info->Mips[i].CompressSize;
						Mips[i].Data.ElementCount = Info->Mips[i].UncompressSize;
					}
					else
					{
						Mips[i].Data.BulkDataFlags = BULKDATA_Unused;
					}
				}
			}
		}
	}
}

FName ULegacyTexture2D::GetLegacyClassName_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	return ULegacyObject::GetLegacyClassName_Implementation(EngineType, GameType);
}

UObject* ULegacyTexture2D::ExportToContent()
{
	if(PresentObject)
	{
		return PresentObject;
	}
	const FString ObjectPath = GetOutContentPath()/ GetLegacyFullName().Replace(TEXT("."),TEXT("/"));
    const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
    const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
    UTexture2D* Texture2D = LoadObject<UTexture2D>(nullptr, *FullObjectPath,nullptr,LOAD_NoWarn);
    if(!Texture2D)
    {
    	bool IsHDR = false;
    	RedImageTool::RedImage Image;
    	ExportTo(Image,&IsHDR);

    	
    	UPackage*  AssetPackage = CreatePackage(*PackageName);
    	Texture2D = NewObject<UTexture2D>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public|RF_Standalone);
    	Texture2D->PreEditChange(nullptr);
    	FAssetRegistryModule::AssetCreated(Texture2D);

	    switch (LODGroup)
	    {
	    case ELegacyTextureGroup::TEXTUREGROUP_World:
	    	Texture2D->LODGroup = TEXTUREGROUP_World;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_WorldNormalMap:
	    	Texture2D->LODGroup = TEXTUREGROUP_WorldNormalMap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_WorldSpecularColor:
	    case ELegacyTextureGroup::TEXTUREGROUP_WorldSpecular:
	    	Texture2D->LODGroup = TEXTUREGROUP_WorldSpecular;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Character:
	    	Texture2D->LODGroup = TEXTUREGROUP_Character;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_CharacterNormalMap:
	    	Texture2D->LODGroup = TEXTUREGROUP_CharacterNormalMap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_CharacterSpecularColor:
	    case ELegacyTextureGroup::TEXTUREGROUP_CharacterSpecular:
	    	Texture2D->LODGroup = TEXTUREGROUP_CharacterSpecular;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Weapon:
	    	Texture2D->LODGroup = TEXTUREGROUP_Weapon;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_WeaponNormalMap:
	    	Texture2D->LODGroup = TEXTUREGROUP_WeaponNormalMap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_WeaponSpecular:
	    	Texture2D->LODGroup = TEXTUREGROUP_WeaponSpecular;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Vehicle:
	    	Texture2D->LODGroup = TEXTUREGROUP_Vehicle;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_VehicleNormalMap:
	    	Texture2D->LODGroup = TEXTUREGROUP_VehicleNormalMap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_VehicleSpecular:
	    	Texture2D->LODGroup = TEXTUREGROUP_VehicleSpecular;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Cinematic:
	    	Texture2D->LODGroup = TEXTUREGROUP_Cinematic;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Effects:
	    	Texture2D->LODGroup = TEXTUREGROUP_Effects;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_EffectsNotFiltered:
	    	Texture2D->LODGroup = TEXTUREGROUP_EffectsNotFiltered;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Skybox:
	    	Texture2D->LODGroup = TEXTUREGROUP_Skybox;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_UI:
	    	Texture2D->LODGroup = TEXTUREGROUP_UI;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Lightmap:
	    	Texture2D->LODGroup = TEXTUREGROUP_Lightmap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_RenderTarget:
	    	Texture2D->LODGroup = TEXTUREGROUP_RenderTarget;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_MobileFlattened:
	    	Texture2D->LODGroup = TEXTUREGROUP_MobileFlattened;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_ProcBuilding_Face:
	    	Texture2D->LODGroup = TEXTUREGROUP_ProcBuilding_Face;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_ProcBuilding_LightMap:
	    	Texture2D->LODGroup = TEXTUREGROUP_ProcBuilding_LightMap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Shadowmap:
	    	Texture2D->LODGroup = TEXTUREGROUP_Shadowmap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_ColorLookupTable:
	    	Texture2D->LODGroup = TEXTUREGROUP_ColorLookupTable;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Terrain_Heightmap:
	    	Texture2D->LODGroup = TEXTUREGROUP_Terrain_Heightmap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Terrain_Weightmap:
	    	Texture2D->LODGroup = TEXTUREGROUP_Terrain_Weightmap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_ImageBasedReflection:
	    	Texture2D->LODGroup = TEXTUREGROUP_RenderTarget;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Bokeh:
	    	Texture2D->LODGroup = TEXTUREGROUP_Bokeh;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_WorldGroundNormal:
	    	Texture2D->LODGroup = TEXTUREGROUP_WorldNormalMap;
		    break;
	    case ELegacyTextureGroup::TEXTUREGROUP_WorldGround:
	    	Texture2D->LODGroup = TEXTUREGROUP_World;
	    	break;
	    case ELegacyTextureGroup::TEXTUREGROUP_Gobo:
	    	Texture2D->LODGroup = TEXTUREGROUP_World;
	    	break;
	    default: ;
	    }
    	if (Texture2D->LODGroup == TEXTUREGROUP_WorldNormalMap)
    	{
    		Texture2D->CompressionSettings = TC_Normalmap;
    	} 
    	else if (IsHDR)
    	{
    		Texture2D->CompressionSettings = TC_HDR;
    	}
    	Texture2D->Source.Init(Image.GetWidth(), Image.GetHeight(), 1, Image.GetMips(), IsHDR?TSF_RGBA16F:TSF_BGRA8, static_cast<uint8*>(*Image));
        Texture2D->PostEditChange();
        Texture2D->Modify();
        
    }
	PresentObject = Texture2D;
    return Texture2D;
}

void ULegacyTexture2D::ExportTo(RedImageTool::RedImage& Image,bool*IsHDR)
{
	int32 MipStart = 0;
	for(MipStart = 0 ;MipStart<Mips.Num();MipStart++)
	{
		if(!(Mips[MipStart].Data.BulkDataFlags& BULKDATA_Unused))
		{
			break;
		}
	}
	if(MipStart == Mips.Num())
	{
		return;
	}
	RedImageTool::RedTexturePixelFormat PixelFormat = Convert(Format,IsHDR);
	Image.Create(Mips[MipStart].SizeX,Mips[MipStart].SizeY,Mips.Num()-MipStart,1,PixelFormat);

	uint8* PixelData = static_cast<uint8*>(*Image);
	for(int32 i = MipStart ;i<Mips.Num();i++)
	{
		if(!Mips[i].Data.BulkData)
		{
			Mips[i].Data.LoadFromFile(TextureFileCacheName.ToString() );
		}
		if(Mips[i].Data.BulkData)
		{
			FMemory::Memcpy(PixelData,Mips[i].Data.BulkData,Mips[i].Data.ElementCount);
		}
		PixelData+=Mips[i].Data.ElementCount;
	}
	switch (PixelFormat)
	{
		case RedImageTool::RedTexturePixelFormat::BC5:
			{
				//sqrt(max(1.0 - normal.x * normal.x - normal.y * normal.y, 0.0))
				Image.Convert(RedImageTool::RedTexturePixelFormat::R32G32B32F);
				FVector3f* Pixels = static_cast<FVector3f*>(*Image);
				for (size_t d = 0; d < Image.GetDepth(); d++)
				{
					for (size_t i = 0; i < Image.GetMips(); i++)
					{
						size_t MipWidth = RedImageTool::RedTextureUtils::GetMip(Image.GetWidth(), i);
						size_t MipHeight = RedImageTool::RedTextureUtils::GetMip(Image.GetHeight(), i);
						for (size_t x = 0; x<MipWidth*MipHeight; x++)
						{
							Pixels[x].X = Pixels[x].X*2 - 1.f;
							Pixels[x].Y = Pixels[x].Y*2 - 1.f;
							Pixels[x].Z = FMath::Sqrt(FMath::Max(1.f - 	Pixels[x].X*	Pixels[x].X-	Pixels[x].Y*	Pixels[x].Y,0.f));
							Pixels[x] = Pixels[x].GetSafeNormal();
							
							Pixels[x].X = Pixels[x].X + 1.f;
							Pixels[x].Y = Pixels[x].Y + 1.f;
							Pixels[x].Z = Pixels[x].Z + 1.f;
							Pixels[x] *= 0.5f;
						}
						Pixels += MipWidth*MipHeight;
					}
				}
				Image.Convert(RedImageTool::RedTexturePixelFormat::R8G8B8A8);
				Image.SwapRB();
			}
			break;
	}
	if(*IsHDR)
	{
		Image.Convert(RedImageTool::RedTexturePixelFormat::R32G32B32A32F);
	}
	else
	{
		Image.Convert(RedImageTool::RedTexturePixelFormat::R8G8B8A8);
		Image.SwapRB();
	}
}

FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyTexture2DMipMap& E)
{
	E.Serialize(Ar);
	return Ar;
}
