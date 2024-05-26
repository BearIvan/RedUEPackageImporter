#pragma once
#include "RedUELegacyArchive.h"
#include "RedUELegacyPackage.generated.h"

struct FRedUELegacyGenerationInfo
{
	int32		ExportCount, NameCount;
	int32		NetObjectCount;

	friend FArchive& operator<<(FRedUELegacyArchive &Ar, FRedUELegacyGenerationInfo &I)
	{
		Ar << I.ExportCount << I.NameCount;
		if (Ar.LegacyVer >= 322) // PACKAGE_V3
			Ar << I.NetObjectCount;
		return Ar;
	}
};


class FRedUELegacyPackageFileSummary
{
public:
	
										FRedUELegacyPackageFileSummary	();
	bool								Serialize						(FRedUELegacyArchive &Ar);
	void								Serialize2						(FRedUELegacyArchive& Ar);
	void								Serialize3						(FRedUELegacyArchive& Ar);
	//UNREAL2
	uint32								Tag								= 0;
	uint16								FileVersion						= 0;
	uint16								LicenseeVersion					= 0;
	int32								PackageFlags					= 0;
	int32								NameCount = 0,   NameOffset		= 0;
	int32								ExportCount = 0, ExportOffset	= 0;
	int32								ImportCount = 0, ImportOffset	= 0;
	FGuid								Guid;
	TArray<FRedUELegacyGenerationInfo> 	Generations;
	//UNREAL3
	int32								HeadersSize						= 0;	// used by UE3 for precaching name table
	FString								PackageGroup;							// "None" or directory name
	int32								DependsOffset					= 0;	// number of items = ExportCount
	int32								ImportExportGuidsOffset			= 0;
	int32								ImportGuidsCount				= 0;
	int32								ExportGuidsCount				= 0;
	int32								EngineVersion					= 0;
	int32								CookerVersion					= 0;
	int32								CompressionFlags				= 0;
	TArray<FCompressedChunk>			CompressedChunks;
};
class FRedUELegacyCompactIndex
{
public:
	int32	Value;
	friend FArchive& operator<<(FArchive &Ar, FRedUELegacyCompactIndex &I);
};

#define AR_INDEX(intref)	(*(FRedUELegacyCompactIndex*)&(intref))

USTRUCT()
struct FRedUELegacyObjectExport
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	UObject			*Object = nullptr;
	
	int32			ClassIndex = INDEX_NONE;		
	int32			PackageIndex = INDEX_NONE;		
	FName			ObjectName;
	int32			SerialSize = INDEX_NONE;	
	int32			SerialOffset = INDEX_NONE;
	
	int32			SuperIndex = INDEX_NONE;					// object reference
	uint32			ObjectFlags = 0;
	
	uint32			ExportFlags = 0;
	uint32			ObjectFlags2 = 0 ;				// really, 'uint64 ObjectFlags'
	int32			Archetype = -1;
	TArray<int32>	NetObjectCount;			// generations
	FGuid			Guid;
	int32			PackageFlags = INDEX_NONE;
	int32			U3unk6C = INDEX_NONE;
	void 			Serialize2			(FRedUELegacyArchive& Ar);
	void 			Serialize2X			(FRedUELegacyArchive& Ar);
	void 			Serialize3			(FRedUELegacyArchive& Ar);
	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FRedUELegacyObjectExport &E);
};

USTRUCT()
struct FRedUELegacyObjectImport
{
	GENERATED_BODY()
	
	FName			ClassPackage;
	FName			ClassName;
	int32			PackageIndex;
	FName			ObjectName;
	bool			Missing;
	void			Serialize(FRedUELegacyArchive& Ar);
};

struct FRedUELegacyObjectDepends
{
	TArray<int>	Objects;
	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FRedUELegacyObjectDepends &D)
	{
		return Ar << D.Objects;
	}
};
UCLASS()
class URedUELegacyPackage:public UObject,public FRedUELegacyArchive
{
	GENERATED_BODY()
public:
									URedUELegacyPackage	();
	void							BeginDestroy		() override;
	bool							LoadPackage			(const TCHAR*FileName);
	void							ClosePackage		();
	
	bool							AtEnd				() override;
	FArchive&						operator<<			(FName& Value) override;
	FArchive&						operator<<			(UObject*& Value) override;
	void							Seek				(int64 InPos) override;
	int64							Tell				() override;
	int64							TotalSize			() override;
	void							Serialize			(void* V, int64 Length) override;

	FRedUELegacyPackageFileSummary	Summary;
	bool							bIsLoaded = false;
private:
	IFileHandle*					FileHandle; 
};
