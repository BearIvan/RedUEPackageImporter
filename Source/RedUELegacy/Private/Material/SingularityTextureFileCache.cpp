#include "SingularityTextureFileCache.h"

#include "Core/RedUELegacySubsystem.h"
#include "HAL/FileManagerGeneric.h"

FArchive& operator<<(FArchive& Ar, FSingularityTextureFileCacheInfo& SingularityTextureFileCacheInfo)
{
	for (auto&i : SingularityTextureFileCacheInfo.Mips)
	{
		Ar << i.OffsetInFile << i.CompressSize << i.UncompressSize << i.Reserve;
	}
	Ar<< SingularityTextureFileCacheInfo.Name;
	return Ar;
}

FSingularityTextureFileCacheInfo* USingularityTextureFileCache::Get(const FGuid& Guid)
{
	if (TFCTextureMap.Num() == 0 && !bFailed)
	{
		Load();
	}
	return TFCTextureMap.Find(Guid);
	
}

void USingularityTextureFileCache::Load()
{
	for (const FString&InContentPath :GetTypedOuter<URedUELegacySubsystem>()->InContentPaths)
	{
		FString Path = FPaths::Combine( InContentPath,TEXT("coalesced.tfc"));
		if(FArchive *TFCReader = IFileManager::Get().CreateFileReader(*Path))
		{
			TFCReader->Seek(0x4AD50000);
			(*TFCReader) << TFCTextureMap;
			delete TFCReader;
			return;
		}
	}
	bFailed = true;
	
}
