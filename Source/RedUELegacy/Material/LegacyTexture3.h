#pragma once

#include "Core/LegacyObject.h"
#include "Core/LegacyPackage.h"
#include "LegacyTexture3.generated.h"


namespace RedImageTool
{
	enum class RedTexturePixelFormat;
}

UENUM()
enum class ELegacyTextureCompressionSettings
{
	TC_Default,
	TC_Normalmap,
	TC_Masks,					// UE4
	TC_Grayscale,
	TC_Displacementmap,
	TC_VectorDisplacementmap,
	TC_HDR,						// UE4
	TC_HighDynamicRange = TC_HDR,
	TC_EditorIcon,				// UE4
	TC_Alpha,					// UE4
	TC_DistanceFieldFont,		// UE4
	TC_HDR_Compressed,			// UE4
	TC_BC7,						// UE4

	TC_NormalmapAlpha,
	TC_OneBitAlpha,
	TC_NormalmapUncompressed,
	TC_NormalmapBC5,
	TC_OneBitMonochrome,
	TC_SimpleLightmapModification,
};

UENUM()
enum class ELegacyPixelFormat
{
	PF_Unknown,
	PF_A32B32G32R32F,
	PF_A8R8G8B8,			
	PF_G8,
	PF_G16,
	PF_DXT1,
	PF_DXT3,
	PF_DXT5,
	PF_UYVY,
	PF_FloatRGB,			
	PF_FloatRGBA,			
	PF_DepthStencil,		
	PF_ShadowDepth,			
	PF_FilteredShadowDepth, 
	PF_R32F,				
	PF_G16R16,
	PF_G16R16F,
	PF_G16R16F_FILTER,
	PF_G32R32F,
	PF_A2B10G10R10,
	PF_A16B16G16R16,
	PF_D24,
	PF_R16F,
	PF_R16F_FILTER,
	PF_BC5,
	PF_V8U8,
	PF_A1,
	PF_FloatR11G11B10,
	PF_A4R4G4B4,
};


UCLASS()
class REDUELEGACY_API ULegacyTexture3 : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual void						LegacySerialize						(FRedUELegacyArchive& Ar) override;
	virtual FName						GetLegacyClassName_Implementation	(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual bool						LegacySupport_Implementation		(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	
	RedImageTool::RedTexturePixelFormat Convert								(ELegacyPixelFormat InLegacyPixelFormat,bool* OutIsHDR = nullptr);
	
	FRedUELegacyByteBulkData			SourceArt;
};
