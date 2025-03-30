// Tyran


#include "Material/LegacyTextureCube.h"

#include "PackageTools.h"
#include "RedImage.hpp"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Material/LegacyTexture2D.h"

void ULegacyTextureCube::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	Ar.Seek(Ar.GetStopper());
}

FName ULegacyTextureCube::GetLegacyClassName_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	return ULegacyObject::GetLegacyClassName_Implementation(EngineType, GameType);
}

UObject* ULegacyTextureCube::ExportToContent()
{
	if(PresentObject)
	{
		return PresentObject;
	}
	const FString ObjectPath = GetOutContentPath()/ GetLegacyFullName().Replace(TEXT("."),TEXT("/"));
    const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
    const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
	UTextureCube* TextureCube = LoadObject<UTextureCube>(nullptr, *FullObjectPath,nullptr,LOAD_NoWarn);
    if(!TextureCube)
    {
	    if(!ensure(FaceNegX))
	    {
		    return nullptr;
	    }
    	if(!ensure(FacePosX))
    	{
    		return nullptr;
    	}
    	if(!ensure(FaceNegY))
    	{
    		return nullptr;
    	}
    	if(!ensure(FacePosY))
    	{
    		return nullptr;
    	}
    	if(!ensure(FaceNegZ))
    	{
    		return nullptr;
    	}
    	if(!ensure(FacePosZ))
    	{
    		return nullptr;
    	}
    	bool IsHDR = false;
    	
    	RedImageTool::RedImage FinalImage;
	    {
		    RedImageTool::RedImage Image;
    		FaceNegX->ExportTo(Image,&IsHDR);
	    	FinalImage.Create(Image.GetWidth(),Image.GetHeight(),Image.GetMips(),6,Image.GetFormat());
	    	FinalImage.Append(0,0,Image,0,0);
	    }
	    {
	    	RedImageTool::RedImage Image;
	    	FacePosX->ExportTo(Image,&IsHDR);
	    	FinalImage.Append(0,0,Image,1,0);
	    }
	    {
	    	RedImageTool::RedImage Image;
	    	FaceNegY->ExportTo(Image,&IsHDR);
	    	FinalImage.Append(0,0,Image,2,0);
	    }
	    {
	    	RedImageTool::RedImage Image;
	    	FacePosY->ExportTo(Image,&IsHDR);
	    	FinalImage.Append(0,0,Image,3,0);
	    }
	    {
	    	RedImageTool::RedImage Image;
	    	FaceNegZ->ExportTo(Image,&IsHDR);
	    	FinalImage.Append(0,0,Image,4,0);
	    }
	    {
	    	RedImageTool::RedImage Image;
	    	FacePosZ->ExportTo(Image,&IsHDR);
	    	FinalImage.Append(0,0,Image,5,0);
	    }
    	
        UPackage*  AssetPackage = CreatePackage(*PackageName);
        TextureCube = NewObject<UTextureCube>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public|RF_Standalone);
        TextureCube->PreEditChange(nullptr);
        FAssetRegistryModule::AssetCreated(TextureCube);
    	ETextureSourceFormat SourceFormat = IsHDR?TSF_RGBA16F:TSF_BGRA8;
		ETextureSourceFormat SourceFormats[6]={ SourceFormat ,SourceFormat ,SourceFormat ,SourceFormat ,SourceFormat ,SourceFormat };
    	TextureCube->Source.InitLayered(FinalImage.GetWidth(), FinalImage.GetHeight(), 6,1, FinalImage.GetMips(), SourceFormats, static_cast<uint8*>(*FinalImage));
		
        TextureCube->PostEditChange();
        TextureCube->Modify();
    	PresentObject = TextureCube;
    }
    return TextureCube;
}
