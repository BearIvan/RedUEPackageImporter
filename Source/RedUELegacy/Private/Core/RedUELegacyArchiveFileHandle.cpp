#include "RedUELegacyArchiveFileHandle.h"

#include "zlib.h"
#include "lzx.h"
#include "mspack.h"
#include "HAL/FileManagerGeneric.h"
#include "lzo/lzo1x.h"
#include "lzo/lzoconf.h"


IFileHandle& operator<<(IFileHandle &Ar, FRedUELegacyCompressedChunkBlock &B)
{
    Ar.Read(reinterpret_cast<uint8*>(&B.CompressedSize),4);
    Ar.Read(reinterpret_cast<uint8*>(&B.UncompressedSize),4);
	return Ar;
}
FArchive& operator<<(FArchive &Ar, FRedUELegacyCompressedChunkBlock &B)
{
	Ar<<B.CompressedSize;
	Ar<<B.UncompressedSize;
	return Ar;
}

IFileHandle& operator<<(IFileHandle &Ar, FRedUELegacyCompressedChunkHeader &H)
{
    Ar.Read(reinterpret_cast<uint8*>(&H.Tag),4);
	check(H.Tag == PACKAGE_FILE_TAG);
    Ar.Read(reinterpret_cast<uint8*>(&H.BlockSize),4);
    Ar<<H.Sum;
	H.BlockSize = 0x20000;
	H.Blocks.Empty((H.Sum.UncompressedSize + 0x20000 - 1) / 0x20000);	// optimized for block size 0x20000
	int32 CompSize = 0, UncompressedSize = 0;
	while (CompSize < H.Sum.CompressedSize && UncompressedSize < H.Sum.UncompressedSize)
	{
		FRedUELegacyCompressedChunkBlock &Block = H.Blocks.AddDefaulted_GetRef();
		Ar << Block;
		CompSize   += Block.CompressedSize;
		UncompressedSize += Block.UncompressedSize;
	}
	// check header; seen one package where sum(Block.CompressedSize) < H.CompressedSize,
	// but UncompressedSize is exact
	check(/*CompSize == H.CompressedSize &&*/ UncompressedSize == H.Sum.UncompressedSize);
	if (H.Blocks.Num() > 1)
		H.BlockSize = H.Blocks[0].UncompressedSize;
	return Ar;
}
FArchive& operator<<(FArchive &Ar, FRedUELegacyCompressedChunkHeader &H)
{
	Ar<<H.Tag;
	check(H.Tag == PACKAGE_FILE_TAG);
	Ar<<H.BlockSize;
	Ar<<H.Sum;
	H.BlockSize = 0x20000;
	H.Blocks.Empty((H.Sum.UncompressedSize + 0x20000 - 1) / 0x20000);	// optimized for block size 0x20000
	int32 CompSize = 0, UncompressedSize = 0;
	while (CompSize < H.Sum.CompressedSize && UncompressedSize < H.Sum.UncompressedSize)
	{
		FRedUELegacyCompressedChunkBlock &Block = H.Blocks.AddDefaulted_GetRef();
		Ar << Block;
		CompSize   += Block.CompressedSize;
		UncompressedSize += Block.UncompressedSize;
	}
	// check header; seen one package where sum(Block.CompressedSize) < H.CompressedSize,
	// but UncompressedSize is exact
	check(/*CompSize == H.CompressedSize &&*/ UncompressedSize == H.Sum.UncompressedSize);
	if (H.Blocks.Num() > 1)
		H.BlockSize = H.Blocks[0].UncompressedSize;
	return Ar;
}

FRedUELegacyArchiveFileHandle::FRedUELegacyArchiveFileHandle(IFileHandle* InFileHandle, int32 CompressionFlags,const TArray<FRedUELegacyCompressedChunk>& CompressedChunks)
    : CompressionFlags(CompressionFlags), CompressedChunks(CompressedChunks), FileHandle(InFileHandle), ChunkHeader(), Position(0),
      BufferStart(0), BufferEnd(0), ChunkDataPos(0)
{
}

FRedUELegacyArchiveFileHandle::~FRedUELegacyArchiveFileHandle()
{
}

int64 FRedUELegacyArchiveFileHandle::Tell()
{
    return Position;
}

bool FRedUELegacyArchiveFileHandle::Seek(int64 NewPosition)
{
    Position = NewPosition;
    return true;
}

bool FRedUELegacyArchiveFileHandle::SeekFromEnd(int64 NewPositionRelativeToEnd)
{
    const FRedUELegacyCompressedChunk &Chunk = CompressedChunks[CompressedChunks.Num() - 1];
#if 0
    if (Game == GAME_Bioshock && ArLicenseeVer >= 57)		//?? Bioshock 2; no version code found
        {
        // Bioshock 2 has added "UncompressedSize" for block, so we must read it
        int OldPos = Reader->Tell();
        int CompressedSize, UncompressedSize;
        Reader->Seek(Chunk.CompressedOffset);
        *Reader << CompressedSize << UncompressedSize;
        Reader->Seek(OldPos);	// go back
        return Chunk.UncompressedOffset + UncompressedSize;
        }
#endif // BIOSHOCK
   return Seek(Chunk.UncompressedOffset + Chunk.UncompressedSize - NewPositionRelativeToEnd);
}

bool FRedUELegacyArchiveFileHandle::Read(uint8* Destination, int64 BytesToRead)
{
    while (true)
    {
        // check for valid buffer
        if (Position >= BufferStart && Position < BufferEnd)
        {
            int32 ToCopy = BufferEnd - Position;	
            if (ToCopy > BytesToRead) ToCopy = BytesToRead;
            FMemory::Memcpy(Destination, Buffer.GetData() + Position - BufferStart, ToCopy);	
            // advance pointers/counters
            Position += ToCopy;
            BytesToRead -= ToCopy;
            Destination     = Destination + ToCopy;
            if (!BytesToRead) return true;
        }
        PrepareBuffer(Position);
        check(Position >= BufferStart && Position < BufferEnd);
    }
}

bool FRedUELegacyArchiveFileHandle::Write(const uint8* Source, int64 BytesToWrite)
{
    checkNoEntry();
    return false;
}

bool FRedUELegacyArchiveFileHandle::Flush(const bool bFullFlush)
{
    return true;
}

bool FRedUELegacyArchiveFileHandle::Truncate(int64 NewSize)
{
    return false;
}

void FRedUELegacyArchiveFileHandle::PrepareBuffer(int64 InPosition)
{
    const FRedUELegacyCompressedChunk* Chunk = nullptr;
    
    for(FRedUELegacyCompressedChunk&InChunk:CompressedChunks)
    {
        Chunk = &InChunk;
        if (InPosition < Chunk->UncompressedOffset + Chunk->UncompressedSize)
            break;
    }
    
    check(Chunk);
    
    if (InPosition < Chunk->UncompressedOffset)
    {
        Buffer.Empty();
        const int32 Size = Chunk->CompressedOffset;
        Buffer.AddUninitialized(Size);
        BufferStart = 0;
        BufferEnd   = Size;
        FileHandle->Seek(0);
        FileHandle->Read(Buffer.GetData(), Size);
        return;
    }

    	if (Chunk != CurrentChunk)
		{
			// serialize compressed chunk header
			FileHandle->Seek(Chunk->CompressedOffset);
#if 0
			if (Game == GAME_Bioshock)
			{
				// read block size
				int CompressedSize;
				*Reader << CompressedSize;
				// generate ChunkHeader
				ChunkHeader.Blocks.Empty(1);
				FCompressedChunkBlock *Block = new (ChunkHeader.Blocks) FCompressedChunkBlock;
				Block->UncompressedSize = 32768;
				if (ArLicenseeVer >= 57)		//?? Bioshock 2; no version code found
					*Reader << Block->UncompressedSize;
				Block->CompressedSize = CompressedSize;
			}
			else
#endif 
			{
				if (Chunk->CompressedSize != Chunk->UncompressedSize)
				{
				    *FileHandle << ChunkHeader;
				}
				else
				{
					// have seen such block in Borderlands: chunk has CompressedSize==UncompressedSize
					// and has no compression; no such code in original engine
					ChunkHeader.BlockSize = -1;	// mark as uncompressed (checked below)
					ChunkHeader.Sum.CompressedSize = ChunkHeader.Sum.UncompressedSize = Chunk->UncompressedSize;
					ChunkHeader.Blocks.Empty(1);
					FRedUELegacyCompressedChunkBlock &Block = ChunkHeader.Blocks.AddDefaulted_GetRef();
					Block.UncompressedSize = Block.CompressedSize = Chunk->UncompressedSize;
				}
			}
			ChunkDataPos = FileHandle->Tell();
			CurrentChunk = Chunk;
		}
		// find block in ChunkHeader.Blocks
		int ChunkPosition = Chunk->UncompressedOffset;
		int ChunkData     = ChunkDataPos;
		check(ChunkPosition <= InPosition);
		const FRedUELegacyCompressedChunkBlock *Block = NULL;
		for (int BlockIndex = 0; BlockIndex < ChunkHeader.Blocks.Num(); BlockIndex++)
		{
			Block = &ChunkHeader.Blocks[BlockIndex];
			if (ChunkPosition + Block->UncompressedSize > InPosition)
				break;
			ChunkPosition += Block->UncompressedSize;
			ChunkData     += Block->CompressedSize;
		}
		check(Block);
		// read compressed data
		//?? optimize? can share compressed buffer and decompressed buffer between packages
        TArray<uint8> CompressedBlock;
        CompressedBlock.AddUninitialized(Block->CompressedSize);
		
		FileHandle->Seek(ChunkData);
		FileHandle->Read(CompressedBlock.GetData(), Block->CompressedSize);
		// prepare buffer for decompression
		if (Block->UncompressedSize > CompressedBlock.Num())
		{
		    Buffer.SetNum(Block->UncompressedSize);
		}
		if (ChunkHeader.BlockSize != -1)	// my own mark
		{
            const int32 UsedCompressionFlags = CompressionFlags;
			Decompress(CompressedBlock.GetData(), Block->CompressedSize, Buffer.GetData(), Block->UncompressedSize, UsedCompressionFlags);
		}
		else
		{
			// No compression
			check(Block->CompressedSize == Block->UncompressedSize);
			FMemory::Memcpy(Buffer.GetData(), CompressedBlock.GetData(), Block->CompressedSize);
		}
		// setup BufferStart/BufferEnd
		BufferStart = ChunkPosition;
		BufferEnd   = ChunkPosition + Block->UncompressedSize;
    
}


struct mspack_file
{
	uint8*		buf;
	int			bufSize;
	int			pos;
	int			rest;
};

static int mspack_read(mspack_file *file, void *buffer, int bytes)
{

	if (!file->rest)
	{
		// read block header
		if (file->buf[file->pos] == 0xFF)
		{
			// [0]   = FF
			// [1,2] = uncompressed block size
			// [3,4] = compressed block size
			file->rest = (file->buf[file->pos+3] << 8) | file->buf[file->pos+4];
			file->pos += 5;
		}
		else
		{
			// [0,1] = compressed size
			file->rest = (file->buf[file->pos+0] << 8) | file->buf[file->pos+1];
			file->pos += 2;
		}
		if (file->rest > file->bufSize - file->pos)
			file->rest = file->bufSize - file->pos;
	}
	if (bytes > file->rest) bytes = file->rest;
	if (bytes <= 0) return 0;

	// copy block data
	memcpy(buffer, file->buf + file->pos, bytes);
	file->pos  += bytes;
	file->rest -= bytes;

	return bytes;
}

static int mspack_write(mspack_file *file, void *buffer, int bytes)
{
	check(file->pos + bytes <= file->bufSize);
	memcpy(file->buf + file->pos, buffer, bytes);
	file->pos += bytes;
	return bytes;
}

static void *mspack_alloc(mspack_system *self, size_t bytes)
{
	return FMemory::Malloc(bytes);
}

static void mspack_free(void *ptr)
{
	FMemory::Free(ptr);
}

static void mspack_copy(void *src, void *dst, size_t bytes)
{
	memcpy(dst, src, bytes);
}

static struct mspack_system lzxSys =
{
	NULL,				// open
	NULL,				// close
	mspack_read,
	mspack_write,
	NULL,				// seek
	NULL,				// tell
	NULL,				// message
	mspack_alloc,
	mspack_free,
	mspack_copy
};


void FRedUELegacyArchiveFileHandle::Decompress(const uint8* CompressedBuffer, int CompressedSize, uint8* UncompressedBuffer,int32 UncompressedSize, int32 Flags)
{
	if (Flags == 0xFF && CompressedSize >= 2)
	{
		uint8 b1 = CompressedBuffer[0];
		uint8 b2 = CompressedBuffer[1];
		if ( b1 == 0x78 &&(b2 == 0x9C || b2 == 0xDA) )	
		{
			Flags = 0x1;
		}
		else
		{
			Flags = 0x2;		// LZO was used only with UE3 games as standard compression method
		}
	}
    if (Flags == 1)
    {
        unsigned long newLen = UncompressedSize;
        check(uncompress(UncompressedBuffer, &newLen, CompressedBuffer, CompressedSize) != Z_OK);
    }
    else if (Flags == 2)
    {
        int r = lzo_init();
        check (r == LZO_E_OK);
        lzo_uint newLen = UncompressedSize;
        r = lzo1x_decompress_safe(CompressedBuffer, CompressedSize, UncompressedBuffer, &newLen, NULL);
        if (r != LZO_E_OK)
        {
          
            checkf(CompressedSize == UncompressedSize,TEXT("lzo_decompress(%d,%d) returned %d"), CompressedSize, UncompressedSize, r);
            {
                memcpy(UncompressedBuffer, CompressedBuffer, UncompressedSize);
                return;
            }
        }
        checkf (newLen == UncompressedSize, TEXT("len mismatch: %d != %d"), newLen, UncompressedSize);
        return;
    }
    else if (Flags == 0x4)
    {
    	mspack_file src, dst;
    	src.buf     = const_cast<uint8*>(CompressedBuffer);
    	src.bufSize = CompressedSize;
    	src.pos     = 0;
    	src.rest    = 0;
    	dst.buf     = UncompressedBuffer;
    	dst.bufSize = UncompressedSize;
    	dst.pos     = 0;
    	lzxd_stream *lzxd = lzxd_init(&lzxSys, &src, &dst, 17, 0, 256*1024, UncompressedSize);
    	check(lzxd);
    	int r = lzxd_decompress(lzxd, UncompressedSize);
    	if (r != MSPACK_ERR_OK)
    	{
    		checkf(false,TEXT("lzxd_decompress(%d,%d) returned %d"), CompressedSize, UncompressedSize, r);
    	}
    	lzxd_free(lzxd);
    }
    else
    {
        checkNoEntry();
    }
}

void FRedUELegacyArchiveFileHandle::Decompress(FArchive& Ar, uint8* Buffer, int32 Size, int32 Flags)
{
	FRedUELegacyCompressedChunkHeader ChunkHeader;
	Ar << ChunkHeader;
	int32 BufferSize = ChunkHeader.BlockSize * 16;
	TArray<uint8> ReadBuffer;
	ReadBuffer.AddUninitialized(BufferSize);
	for (int BlockIndex = 0; BlockIndex < ChunkHeader.Blocks.Num(); BlockIndex++)
	{
		const FRedUELegacyCompressedChunkBlock *Block = &ChunkHeader.Blocks[BlockIndex];
		check(Block->CompressedSize <= BufferSize);
		check(Block->UncompressedSize <= Size);
		Ar.Serialize(ReadBuffer.GetData(), Block->CompressedSize);
		Decompress(ReadBuffer.GetData(), Block->CompressedSize, Buffer, Block->UncompressedSize, Flags);
		Size   -= Block->UncompressedSize;
		Buffer += Block->UncompressedSize;
	}
}
