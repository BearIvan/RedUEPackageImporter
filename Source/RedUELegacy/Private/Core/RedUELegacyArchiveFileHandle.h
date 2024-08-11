#pragma once
#include "Core/LegacyPackage.h"

struct FRedUELegacyCompressedChunkBlock
{
    int			CompressedSize;
    int			UncompressedSize;

    friend IFileHandle& operator<<(IFileHandle &Ar, FRedUELegacyCompressedChunkBlock &B);
    friend FArchive& operator<<(FArchive &Ar, FRedUELegacyCompressedChunkBlock &B);
};

struct FRedUELegacyCompressedChunkHeader
{
    int32			                            Tag;
    int32			                            BlockSize;				// maximal size of uncompressed block
    FRedUELegacyCompressedChunkBlock            Sum;			// summary for the whole compressed block
    TArray<FRedUELegacyCompressedChunkBlock>    Blocks;

    friend IFileHandle& operator<<(IFileHandle &Ar, FRedUELegacyCompressedChunkHeader &H);
    friend FArchive& operator<<(FArchive &Ar, FRedUELegacyCompressedChunkHeader &H);
};

class FRedUELegacyArchiveFileHandle:public IFileHandle
{
public:
    FRedUELegacyArchiveFileHandle(IFileHandle* InFileHandle,int32 CompressionFlags,const TArray<FRedUELegacyCompressedChunk>&CompressedChunks);
    virtual ~FRedUELegacyArchiveFileHandle() override;
    virtual int64 Tell() override;
    virtual bool Seek(int64 NewPosition) override;
    virtual bool SeekFromEnd(int64 NewPositionRelativeToEnd) override;
    virtual bool Read(uint8* Destination, int64 BytesToRead) override;
    virtual bool Write(const uint8* Source, int64 BytesToWrite) override;
    virtual bool Flush(const bool bFullFlush) override;
    virtual bool Truncate(int64 NewSize) override;
    void PrepareBuffer(int64 Position);
    static void Decompress(const uint8 *CompressedBuffer, int CompressedSize, uint8 *UncompressedBuffer, int32 UncompressedSize, int32 Flags);
    static void Decompress(FArchive& Ar, uint8* Buffer, int32 Size, int32 Flags);
    int32								CompressionFlags;
    TArray<FRedUELegacyCompressedChunk>	CompressedChunks;
    IFileHandle*                        FileHandle = nullptr;
    FRedUELegacyCompressedChunkHeader	ChunkHeader;
    
    int32					        	Position;
    
    TArray<uint8>                       Buffer;
    int32						        BufferStart;
    int32						        BufferEnd;
	int32						        ChunkDataPos;
    const FRedUELegacyCompressedChunk*  CurrentChunk = nullptr;
};
