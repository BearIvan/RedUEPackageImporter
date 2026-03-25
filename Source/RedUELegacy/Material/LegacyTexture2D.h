#pragma once

#include "LegacyTexture3.h"
#include "LegacyTexture2D.generated.h"

namespace RedImageTool
{
	class RedImage;
}

UENUM()
enum class ELegacyTextureAddress
{
	TA_Wrap,
	TA_Clamp,
	TA_Mirror
};

struct FLegacyTexture2DMipMap
{
	FRedUELegacyByteBulkData	Data;
	int32						SizeX;
	int32						SizeY;
	void						Serialize	(FRedUELegacyArchive& Ar);
	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyTexture2DMipMap &E);
};
UENUM()
enum class ELegacyTextureGroup
{
	TEXTUREGROUP_World      ,
	TEXTUREGROUP_WorldNormalMap,
	TEXTUREGROUP_WorldSpecular,
	TEXTUREGROUP_Character  ,
	TEXTUREGROUP_CharacterNormalMap,
	TEXTUREGROUP_CharacterSpecular,
	TEXTUREGROUP_Weapon     ,
	TEXTUREGROUP_WeaponNormalMap,
	TEXTUREGROUP_WeaponSpecular,
	TEXTUREGROUP_Vehicle    ,
	TEXTUREGROUP_VehicleNormalMap,
	TEXTUREGROUP_VehicleSpecular,
	TEXTUREGROUP_Cinematic  ,
	TEXTUREGROUP_Effects    ,
	TEXTUREGROUP_EffectsNotFiltered,
	TEXTUREGROUP_Skybox     ,
	TEXTUREGROUP_UI         ,
	TEXTUREGROUP_Lightmap   ,
	TEXTUREGROUP_RenderTarget,
	TEXTUREGROUP_MobileFlattened,
	TEXTUREGROUP_ProcBuilding_Face,
	TEXTUREGROUP_ProcBuilding_LightMap,
	TEXTUREGROUP_Shadowmap,
	TEXTUREGROUP_ColorLookupTable,
	TEXTUREGROUP_Terrain_Heightmap,
	TEXTUREGROUP_Terrain_Weightmap,
	TEXTUREGROUP_ImageBasedReflection,
	TEXTUREGROUP_Bokeh,
	TEXTUREGROUP_WorldGroundNormal,
	TEXTUREGROUP_WorldGround,
	TEXTUREGROUP_Gobo,
	TEXTUREGROUP_WorldSpecularColor,
	TEXTUREGROUP_CharacterSpecularColor,
	TEXTUREGROUP_WeaponSpecularColor
};

UCLASS()
class REDUELEGACY_API ULegacyTexture2D : public ULegacyTexture3
{
	GENERATED_BODY()
public:
	virtual void					LegacySerialize						(FRedUELegacyArchive& Ar) override;
	virtual FName					GetLegacyClassName_Implementation	(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual UObject*				ExportToContent						() override;
	void							ExportTo							(RedImageTool::RedImage& Image,bool*IsHDR = nullptr,bool bIsNormaMap = false);
	FGuid							TextureFileCacheGuid;
	TArray<FLegacyTexture2DMipMap>	Mips;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyTextureCompressionSettings CompressionSettings;

	UPROPERTY(BlueprintReadWrite)
	int32 SizeX;
	
	UPROPERTY(BlueprintReadWrite)
	int32 SizeY;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyPixelFormat Format;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyTextureAddress	AddressX;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyTextureAddress	AddressY;
	
	UPROPERTY(BlueprintReadWrite)
	FName TextureFileCacheName;

	UPROPERTY(BlueprintReadWrite)
	ELegacyTextureGroup LODGroup;
};
