#pragma once

#include "Core/LegacyObject.h"
#include "Entities/Engine/LegacyWorldInfo.h"
#include "UObject/Object.h"
#include "LegacyXWorldInfo.generated.h"



UCLASS()
class REDUEBIOSHOCK3IMPORTER_API UXLevelStreamingGroup: public ULegacyObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FName GroupName;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyLevelStreaming*> StreamingLevels;
};



USTRUCT(Blueprintable)
struct FLegacyXWorldInfoStreamingLevelsWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyLevelStreaming*> StreamingLevels;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<UObject*> GroupedStreamingLevels;
	
};


UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXWorldInfo : public ULegacyWorldInfo
{
	GENERATED_BODY()
public:
	virtual	void	GetStreamingLevels(TArray<ULegacyLevelStreaming*> & OutStreamingLevels) override;
	
	UPROPERTY(BlueprintReadWrite)
	FLegacyXWorldInfoStreamingLevelsWrapper StreamingLevelsWrapper;
};