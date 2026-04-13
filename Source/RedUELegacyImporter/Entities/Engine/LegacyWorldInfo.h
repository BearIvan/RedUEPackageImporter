#pragma once
#include "LegacyInfo.h"
#include "Entities/LegacyActor.h"

#include "LegacyWorldInfo.generated.h"


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyLevelStreaming: public ULegacyObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FName PackageName;
};



UCLASS()
class REDUELEGACYIMPORTER_API ULegacyLevelStreamingAlwaysLoaded: public ULegacyLevelStreaming
{
	GENERATED_BODY()
public:
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyLevelStreamingKismet: public ULegacyLevelStreaming
{
	GENERATED_BODY()
public:
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyZoneInfo : public ULegacyInfo
{
	GENERATED_BODY()
public:
	
	
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyWorldInfo : public ULegacyZoneInfo
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyLevelStreaming*> StreamingLevels;

	virtual	void	GetStreamingLevels(TArray<ULegacyLevelStreaming*> & OutStreamingLevels);
	virtual UClass* GetActorClass_Implementation() override;
	
};