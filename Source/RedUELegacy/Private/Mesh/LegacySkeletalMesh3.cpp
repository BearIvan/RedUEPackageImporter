#include "Mesh/LegacySkeletalMesh3.h"

ULegacySkeletalMesh3::ULegacySkeletalMesh3()
{
}

ULegacySkeletalMesh3::~ULegacySkeletalMesh3()
{
}

FName ULegacySkeletalMesh3::GetLegacyClassName_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	return Super::GetLegacyClassName_Implementation(EngineType, GameType);
}

bool ULegacySkeletalMesh3::LegacySupport_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	return Super::LegacySupport_Implementation(EngineType, GameType);
}

void ULegacySkeletalMesh3::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
}

UObject* ULegacySkeletalMesh3::ExportToContent()
{
	return Super::ExportToContent();
	
}
