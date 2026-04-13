#pragma once
#include "RedUELegacyImporterSettings.generated.h"
class ULegacyObject;

UCLASS(Config = RedUELegacyEditor, defaultconfig)
class REDUELEGACYIMPORTER_API URedUELegacyImporterSettings :public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Config,EditAnywhere)
	TMap<FName,FName> EmptyClasses;
	
	UPROPERTY(Config,EditAnywhere)
	TSet<TSoftClassPtr<ULegacyObject>> IgnoredClassesForAutoMakeEmptyClass;
	
	UPROPERTY(Config,EditAnywhere)
	bool bAutoMakeEmptyClass = true;
	
	UPROPERTY(Config,EditAnywhere)
	FString Language = TEXT("RUS");
};
