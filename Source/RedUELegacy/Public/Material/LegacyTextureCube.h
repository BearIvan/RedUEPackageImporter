#pragma once

#include "LegacyTexture3.h"
#include "LegacyTextureCube.generated.h"

/**
 * 
 */
UCLASS()
class REDUELEGACY_API ULegacyTextureCube : public ULegacyTexture3
{
	GENERATED_BODY()
public:
	virtual void					LegacySerialize						(FRedUELegacyArchive& Ar) override;
	virtual FName					GetLegacyClassName_Implementation	(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual UObject*				ExportToContent						();

	UPROPERTY(BlueprintReadWrite)
	ULegacyTexture2D* FacePosX;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyTexture2D* FaceNegX;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyTexture2D* FacePosY;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyTexture2D* FaceNegY;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyTexture2D* FacePosZ;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyTexture2D* FaceNegZ;
};
