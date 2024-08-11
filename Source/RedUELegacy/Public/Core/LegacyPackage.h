#pragma once
#include "RedUELegacyArchive.h"
#include "LegacyPackage.generated.h"

class ULegacyObject;

struct FRedUELegacyGenerationInfo
{
	int32		ExportCount, NameCount;
	int32		NetObjectCount;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FRedUELegacyGenerationInfo &I)
	{
		Ar << I.ExportCount << I.NameCount;
		if (Ar.LegacyVer >= 322) // PACKAGE_V3
			Ar << I.NetObjectCount;
		return Ar;
	}
};

struct FRedUELegacyCompressedChunk
{

    /** Original offset in uncompressed file.	*/
    int32		UncompressedOffset;
    /** Uncompressed size in bytes.				*/
    int32		UncompressedSize;
    /** Offset in compressed file.				*/
    int32		CompressedOffset;
    /** Compressed size in bytes.				*/
    int32		CompressedSize;

    /** I/O functions */
    friend  FArchive& operator<<(FArchive& Ar, FRedUELegacyCompressedChunk& Chunk)
    {
        Ar << Chunk.UncompressedOffset << Chunk.UncompressedSize << Chunk.CompressedOffset << Chunk.CompressedSize;
        return Ar;
    }
};

// struct FRedUECompressedChunkBlock
// {
// 	int			CompressedSize;
// 	int			UncompressedSize;
//
// 	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FRedUECompressedChunkBlock &B)
// 	{
// 		 Ar << B.CompressedSize << B.UncompressedSize;
// 		return Ar;
// 	}
// };
//
// struct FRedUECompressedChunkHeader
// {
// 	int			Tag;
// 	int			BlockSize;			
// 	FRedUECompressedChunkBlock Sum;			
// 	TArray<FRedUECompressedChunkBlock> Blocks;
// 	
// 	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FRedUECompressedChunkHeader &H)
// 	{
// 		Ar << H.Tag;
// 		if (H.Tag == PACKAGE_FILE_TAG_SWAPPED)
// 			Ar.SetByteSwapping(!Ar.ArForceByteSwapping);
// 		check(H.Tag == PACKAGE_FILE_TAG);
// 		
// 		Ar << H.BlockSize;
// 		Ar << H.Sum;
// 		{
// 			H.BlockSize = 0x20000;
// 			H.Blocks.Empty((H.Sum.UncompressedSize + 0x20000 - 1) / 0x20000);	// optimized for block size 0x20000
// 			int CompSize = 0, UncompressedSize = 0;
// 			while (CompSize < H.Sum.CompressedSize && UncompressedSize < H.Sum.UncompressedSize)
// 			{
// 				FRedUECompressedChunkBlock &Block = H.Blocks.AddDefaulted_GetRef();
// 				Ar << Block;
// 				CompSize   += Block.CompressedSize;
// 				UncompressedSize += Block.UncompressedSize;
// 			}
// 			check( UncompressedSize == H.Sum.UncompressedSize);
// 			if (H.Blocks.Num() > 1)
// 				H.BlockSize = H.Blocks[0].UncompressedSize;
// 		}
// 		return Ar;
// 	}
//
// };

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
	TArray<FRedUELegacyCompressedChunk>	CompressedChunks;
};


USTRUCT()
struct FRedUELegacyObjectExport
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	ULegacyObject	*Object = nullptr;
	
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
	
	UPROPERTY(Transient)
	ULegacyObject	*Object = nullptr;
	
	FName			ClassPackage;
	FName			ClassName;
	int32			PackageIndex;
	FName			ObjectName;
	bool			Missing = false;
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


enum class ERedUEBulkDataFlags:uint32
{
    StoreInSeparateFile  =	0x01,		// bulk stored in different file (otherwise it's "inline")
    CompressedZlib		 =	0x02,		// name = BULKDATA_SerializeCompressedZLIB (UE4) ?
    CompressedLzo		 =	0x10,		// unknown name
    Unused				 =	0x20,		// empty bulk block
    SeparateData		 =	0x40,		// unknown name - bulk stored in a different place in the same file
    CompressedLzx		 =	0x80,		// unknown name
};
ENUM_CLASS_FLAGS(ERedUEBulkDataFlags)

constexpr uint32  operator& (uint32  Lhs, ERedUEBulkDataFlags Rhs)
{
    return (Lhs & static_cast<uint32>(Rhs));
} 
	
constexpr uint32  operator|= (uint32  Lhs, ERedUEBulkDataFlags Rhs)
{
    return (Lhs | static_cast<uint32>(Rhs));
}

struct FRedUELegacyByteBulkData 
{
    uint32	BulkDataFlags;				
    int32	ElementCount;				
    int64	BulkDataOffsetInFile = 0;		
    int32	BulkDataSizeOnDisk;			
    uint8	*BulkData = nullptr;
    virtual ~FRedUELegacyByteBulkData()
    {
        ReleaseData();
    }
    virtual int GetElementSize() const
    {
        return 1;
    }

    void ReleaseData()
    {
        if (BulkData) FMemory::Free(BulkData);
        BulkData = nullptr;
    }
    
    bool CanReloadBulk() const
    {
        return (BulkDataFlags & ERedUEBulkDataFlags::StoreInSeparateFile) != 0;
    }
    
    void SerializeHeader(FRedUELegacyArchive &Ar);
    void SerializeData(FArchive &Ar);
    void Serialize(FRedUELegacyArchive &Ar);
    void Skip(FRedUELegacyArchive &Ar);
	void LoadFromFile(const FString&Name);
protected:
    void SerializeDataChunk(FArchive &Ar);
};

UCLASS(BlueprintType)
class REDUELEGACY_API ULegacyPackage:public UObject,public FRedUELegacyArchive
{
	GENERATED_BODY()
public:
									ULegacyPackage	    ();
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
    virtual int32                   GetStopper          () override;
    virtual void                    SetStopper          (int32 Pos) override;
    virtual void                    PushStopper         () override;
    virtual void                    PopStopper          () override;
    
	ULegacyObject*                  GetOrCreateImport	(int32 Index);
    ULegacyObject*                  GetOrCreateExport   (int32 Index);
	FName							GetObjectPackageName(int PackageIndex) const;
    int32                           FindExport          (FName Name);
    FString                         GetFullExportName   (int32 Index);
    void                            SetupReader         (int32 ExportIndex);

    FRedUELegacyObjectImport& GetImport(int32 index)
    {
        checkf(index >=0 && index < Summary.ImportCount,TEXT( "Package \"%s\": wrong import index %d"), *FileName, index);
        return ImportTable[index];
    }

    const FRedUELegacyObjectImport& GetImport(int32 index) const
    {
        checkf(index >=0 && index < Summary.ImportCount,TEXT( "Package \"%s\": wrong import index %d"), *FileName, index);
        return ImportTable[index];
    }

    FRedUELegacyObjectExport& GetExport(int32 index)
    {
        checkf(index >=0 && index < Summary.ExportCount,TEXT( "Package \"%s\": wrong export index %d"), *FileName, index);
        return ExportTable[index];
    }

    const FRedUELegacyObjectExport& GetExport(int index) const
    {
        checkf(index >=0 && index < Summary.ExportCount,TEXT( "Package \"%s\": wrong export index %d"), *FileName, index);
        return ExportTable[index];
    }

    FName GetObjectName(int32 PackageIndex) const	//?? GetExportClassName()
    {
        if (PackageIndex < 0)
        {
            return GetImport(-PackageIndex-1).ObjectName;
        }
        else if (PackageIndex > 0)
        {
            return GetExport(PackageIndex-1).ObjectName;
        }
        else 
        {
            return NAME_Class;
        }
    }
    
    FRedUELegacyPackageFileSummary	Summary;
    bool							bIsLoaded = false;
    FString                         FileName;

    UPROPERTY(BlueprintReadOnly)
    ERedUELegacyGameType GameType;
    
    UPROPERTY(BlueprintReadOnly)
    ERedUELegacyEngineType EngineType;
    
private:
    void                            LoadNameTable       ();
    void                            LoadImportTable     ();
    void                            LoadExportTable     ();

    TArray<FString>                 NameTable;
	IFileHandle*					FileHandle;
    int32                           Stopper = INDEX_NONE;
    TArray<int32>                   StoppersSaved;
    
    UPROPERTY(Transient)
	TArray<FRedUELegacyObjectExport> ExportTable;
	
	UPROPERTY(Transient)
	TArray<FRedUELegacyObjectImport>ImportTable;
};
