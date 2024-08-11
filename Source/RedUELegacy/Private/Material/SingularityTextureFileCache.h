#pragma once

#include "SingularityTextureFileCache.generated.h"

#pragma pack(push,4)
struct FSingularityTextureFileCacheInfo
{
	struct
	{
		uint64 OffsetInFile;
		uint32 UncompressSize;
		uint32 CompressSize;
		uint64 Reserve;
	} Mips[6];
	FString Name;
};
#pragma pack(pop)

UCLASS()
class REDUELEGACY_API USingularityTextureFileCache : public UObject
{
	GENERATED_BODY()
public:
	FSingularityTextureFileCacheInfo*				Get				(const FGuid&Guid);
private:			
	void											Load			();
	bool											bFailed = false;
	TMap<FGuid, FSingularityTextureFileCacheInfo>	TFCTextureMap;
};
