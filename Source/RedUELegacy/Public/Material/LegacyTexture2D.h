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

UCLASS()
class REDUELEGACY_API ULegacyTexture2D : public ULegacyTexture3
{
	GENERATED_BODY()
public:
	virtual void					LegacySerialize						(FRedUELegacyArchive& Ar) override;
	virtual FName					GetLegacyClassName_Implementation	(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual UObject*				ExportToContent						();
	void							ExportTo							(RedImageTool::RedImage& Image,bool*IsHDR = nullptr);
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
};
