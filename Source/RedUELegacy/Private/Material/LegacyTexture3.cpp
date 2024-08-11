#include "Material/LegacyTexture3.h"

#include "RedImage.hpp"
#include "Core/RedUELegacyGame.h"

void ULegacyTexture3::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	SourceArt.Serialize(Ar);
}

FName ULegacyTexture3::GetLegacyClassName_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	static FName NAME_Texture = "Texture";
	return NAME_Texture;
}

bool ULegacyTexture3::LegacySupport_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	if (EngineType != ERedUELegacyEngineType::UnrealEngine3)
	{
		return false;
	}
	return Super::LegacySupport_Implementation(EngineType, GameType);
}

RedImageTool::RedTexturePixelFormat ULegacyTexture3::Convert(ELegacyPixelFormat InLegacyPixelFormat, bool* OutIsHDR)
{
	switch (InLegacyPixelFormat)
	{
	case ELegacyPixelFormat::PF_A32B32G32R32F:
		*OutIsHDR = true;
		return RedImageTool::RedTexturePixelFormat::R32G32B32A32F;
	case ELegacyPixelFormat::PF_A8R8G8B8:
		*OutIsHDR = true;
		return RedImageTool::RedTexturePixelFormat::R8G8B8A8;
	case ELegacyPixelFormat::PF_G8:
		return RedImageTool::RedTexturePixelFormat::R8;
	case ELegacyPixelFormat::PF_DXT1:
		return RedImageTool::RedTexturePixelFormat::DXT_1;
	case ELegacyPixelFormat::PF_DXT3:
		return RedImageTool::RedTexturePixelFormat::DXT_3;
	case ELegacyPixelFormat::PF_DXT5:
		return RedImageTool::RedTexturePixelFormat::DXT_5;
	case ELegacyPixelFormat::PF_FloatRGB:
		*OutIsHDR = true;
		return RedImageTool::RedTexturePixelFormat::R32G32B32F;
	case ELegacyPixelFormat::PF_FloatRGBA:
		*OutIsHDR = true;
		return RedImageTool::RedTexturePixelFormat::R32G32B32A32F;
	case ELegacyPixelFormat::PF_R32F:
		*OutIsHDR = true;
		return RedImageTool::RedTexturePixelFormat::R32F;
	case ELegacyPixelFormat::PF_G32R32F:
		*OutIsHDR = true;
		return RedImageTool::RedTexturePixelFormat::R32G32F;
	case ELegacyPixelFormat::PF_BC5:
		return RedImageTool::RedTexturePixelFormat::BC5;
	case ELegacyPixelFormat::PF_V8U8:
		return RedImageTool::RedTexturePixelFormat::R8G8;
	default:
		checkNoEntry();
		return RedImageTool::RedTexturePixelFormat::None;
	}
}
