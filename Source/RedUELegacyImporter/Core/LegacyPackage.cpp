#include "Core\LegacyPackage.h"

#include "Editor.h"
#include "LegacyTypeInfo.h"
#include "RedUELegacyArchiveFileHandle.h"
#include "RedUELegacyImporterSettings.h"
#include "Core/RedUELegacySubsystem.h"
#include "Kismet/Base/SequenceAction.h"
#include "Sequence/LegacySequenceObjects.h"
#include "World/LegacyWorld.h"

FRedUELegacyPackageFileSummary::FRedUELegacyPackageFileSummary()
{
}

bool FRedUELegacyPackageFileSummary::Serialize(FRedUELegacyArchive& Ar)
{
	check(Ar.IsLoading());
	// read package tag
	Ar << Tag;

	if (Tag != PACKAGE_FILE_TAG)
	{
		if (Tag != PACKAGE_FILE_TAG_SWAPPED)
		{
			ensureMsgf(false,TEXT("Wrong package tag (%08X) in file %s. Probably the file is encrypted."), Tag, *Ar.GetArchiveName());
			return false;
		}
		Ar.ArForceByteSwapping = true;
		Tag = PACKAGE_FILE_TAG;
	}
	uint32_t Version;
	Ar << Version;

	FileVersion     = Version & 0xFFFF;
	LicenseeVersion = Version >> 16;
	// store file version to archive (required for some structures, for UNREAL3 path)
	Ar.LegacyVer         = FileVersion;
	Ar.LegacyLicenseeVer = LicenseeVersion;
	Ar.DetectGame();
	if (Ar.Game >= ERedUELegacyGame::UE3)
		Serialize3(Ar);
	else
		Serialize2(Ar);
	return true;
	
}

void FRedUELegacyPackageFileSummary::Serialize2(FRedUELegacyArchive& Ar)
{
	Ar << PackageFlags;
	Ar << NameCount << NameOffset << ExportCount << ExportOffset << ImportCount << ImportOffset;
	// Guid and generations
	if (Ar.LegacyVer < 68)
	{
		// old generations code
		int32 HeritageCount, HeritageOffset;
		Ar << HeritageCount << HeritageOffset;	// not used
		if (Ar.IsLoading())
		{
			Generations.Empty(1);
			FRedUELegacyGenerationInfo gen;
			gen.ExportCount = ExportCount;
			gen.NameCount   = NameCount;
			Generations.Add(gen);
		}
	}
	else
	{
		Ar << Guid;
		int32 Count;
		Ar << Count;
		Generations.Empty(Count);
		Generations.AddZeroed(Count);
		for (int i = 0; i < Count; i++)
			Ar << Generations[i];
	}

}

void FRedUELegacyPackageFileSummary::Serialize3(FRedUELegacyArchive& Ar)
{
	if (Ar.Game == ERedUELegacyGame::Bioshock3)
	{
		int32 unkC;
		Ar << unkC;
	}
	
	if (Ar.LegacyVer >= 249)
		Ar << HeadersSize;
	else
		HeadersSize = 0;
	
	if (Ar.LegacyVer >= 269)
		Ar << PackageGroup;

	Ar << PackageFlags;
	Ar << NameCount << NameOffset << ExportCount << ExportOffset;
	Ar << ImportCount << ImportOffset;

	if (Ar.LegacyVer >= 415)
		Ar << DependsOffset;
	
	if (Ar.Game != ERedUELegacyGame::Bioshock3)
	{
		if (Ar.LegacyVer >= 623)
			Ar << ImportExportGuidsOffset << ImportGuidsCount << ExportGuidsCount;
	}

	if (Ar.LegacyVer >= 584)
	{
		int32 ThumbnailTableOffset;
		Ar << ThumbnailTableOffset;
	}
	// Guid and generations
	Ar << Guid;
	int32 Count;
	Ar << Count;
	Generations.Empty(Count);
	Generations.AddZeroed(Count);
	for (int i = 0; i < Count; i++)
		Ar << Generations[i];

	if (Ar.LegacyVer >= 245)
		Ar << EngineVersion;
	
	if (Ar.LegacyVer >= 277)
		Ar << CookerVersion;

    if (Ar.LegacyVer >= 334)
        Ar << CompressionFlags << CompressedChunks;
    int32		U3unk60;
    if (Ar.LegacyVer >= 482)
        Ar << U3unk60;
}

FArchive& operator<<(FArchive &Ar, FRedUELegacyCompactIndex &I)
{
	if (Ar.IsLoading())
	{
		uint8 b;
		Ar << b;
		int sign  = b & 0x80;	// sign bit
		int shift = 6;
		int r     = b & 0x3F;
		if (b & 0x40)			// has 2nd byte
			{
			do
			{
				Ar << b;
				r |= (b & 0x7F) << shift;
				shift += 7;
			} while (b & 0x80);	// has more bytes
			}
		I.Value = sign ? -r : r;
	}
	else
	{
		int v = I.Value;
		uint8 b = 0;
		if (v < 0)
		{
			v = -v;
			b |= 0x80;			// sign
		}
		b |= v & 0x3F;
		if (v <= 0x3F)
		{
			Ar << b;
		}
		else
		{
			b |= 0x40;			// has 2nd byte
			v >>= 6;
			Ar << b;
			check(v);
			while (v)
			{
				b = v & 0x7F;
				v >>= 7;
				if (v)
					b |= 0x80;	// has more bytes
				Ar << b;
			}
		}
	}
	return Ar;
}


void FRedUELegacyObjectExport::Serialize2(FRedUELegacyArchive& Ar)
{
	
	if (Ar.Game == ERedUELegacyGame::Bioshock)
	{
		int unkC, flags2, unk30;
		Ar << AR_INDEX(ClassIndex) << AR_INDEX(SuperIndex) << PackageIndex;
		if (Ar.LegacyVer >= 132) Ar << unkC;			// unknown
		Ar << ObjectName << ObjectFlags;
		if (Ar.LegacyLicenseeVer >= 40) Ar << flags2;	// qword flags
		Ar << AR_INDEX(SerialSize);
		if (SerialSize)
			Ar << AR_INDEX(SerialOffset);
		if (Ar.LegacyVer >= 130) Ar << unk30;			// unknown
		return;
	}
	
	// generic UE1/UE2 code
	Ar << AR_INDEX(ClassIndex) << AR_INDEX(SuperIndex) << PackageIndex;
	Ar << ObjectName << ObjectFlags << AR_INDEX(SerialSize);
	if (SerialSize)
		Ar << AR_INDEX(SerialOffset);
}

void FRedUELegacyObjectExport::Serialize2X(FRedUELegacyArchive& Ar)
{
	
	Ar << ClassIndex << SuperIndex;
	if (Ar.LegacyVer >= 150)
	{
		int16 idx = PackageIndex;
		Ar << idx;
		PackageIndex = idx;
	}
	else
		Ar << PackageIndex;
	Ar << ObjectName << ObjectFlags << SerialSize;
	if (SerialSize)
		Ar << SerialOffset;
	// UC2 has strange thing here: indices are serialized as 4-byte int (instead of AR_INDEX),
	// but stored into 2-byte shorts
	ClassIndex = int16(ClassIndex);
	SuperIndex = int16(SuperIndex);

}

void FRedUELegacyObjectExport::Serialize3(FRedUELegacyArchive& Ar)
{
	Ar << ClassIndex << SuperIndex << PackageIndex << ObjectName;
	if (Ar.LegacyVer >= 220) Ar << Archetype;


	Ar << ObjectFlags;
	if (Ar.LegacyVer >= 195) Ar << ObjectFlags2;	// qword flags after version 195
	Ar << SerialSize;
	if (SerialSize || Ar.LegacyVer >= 249)
		Ar << SerialOffset;

	if (Ar.LegacyVer < 543)
	{
		TMap<FName, int> tmpComponentMap;
		Ar << tmpComponentMap;
	}
	
	if (Ar.LegacyVer >= 247) Ar << ExportFlags;
	if (Ar.Game == ERedUELegacyGame::Bioshock3)
	{
		int flag;
		Ar << flag;
		if (!flag) return;	
	}
	if (Ar.LegacyVer >= 322) Ar << NetObjectCount << Guid;
	if (Ar.LegacyVer >= 475) Ar << U3unk6C;

}

void FRedUELegacyObjectImport::Serialize(FRedUELegacyArchive& Ar)
{
	if (EnumHasAllFlags(Ar.Game&ERedUELegacyGame::Engine,ERedUELegacyGame::UE2X) && Ar.LegacyVer >= 150)
	{
		int16 idx = PackageIndex;
		Ar << ClassPackage << ClassName << idx << ObjectName;
		PackageIndex = idx;
		return;
	}
	Ar << ClassPackage << ClassName << PackageIndex << ObjectName;
}

void FRedUELegacyByteBulkData::SerializeHeader(FRedUELegacyArchive& Ar)
{
    if (Ar.LegacyVer < 266)
    {
        BulkDataFlags = 4;						// unknown
        BulkDataSizeOnDisk = INDEX_NONE;
        int32 EndPosition;
        Ar << EndPosition;
        if (Ar.LegacyVer >= 254)
            Ar << BulkDataSizeOnDisk;
        if (Ar.LegacyVer >= 251)
        {
            int LazyLoaderFlags;
            Ar << LazyLoaderFlags;
            check((LazyLoaderFlags & 1) == 0);
            if (LazyLoaderFlags & 2)
                BulkDataFlags |= ERedUEBulkDataFlags::CompressedZlib;
        }
        if (Ar.LegacyVer >= 260)
        {
            FName unk;
            Ar << unk;
        }
        Ar << ElementCount;
        if (BulkDataSizeOnDisk == INDEX_NONE)
            BulkDataSizeOnDisk = ElementCount * GetElementSize();
        BulkDataOffsetInFile = Ar.Tell();
        BulkDataSizeOnDisk   = EndPosition - static_cast<int32>(BulkDataOffsetInFile);
    }
    else
    {
        Ar << BulkDataFlags << ElementCount;
        int32 tmpBulkDataOffsetInFile32;
        Ar << BulkDataSizeOnDisk << tmpBulkDataOffsetInFile32;
        BulkDataOffsetInFile = tmpBulkDataOffsetInFile32;	
    }
    
}

void FRedUELegacyByteBulkData::SerializeData(FArchive& Ar)
{
    check(!(BulkDataFlags & ERedUEBulkDataFlags::Unused));
    if (BulkDataFlags & (ERedUEBulkDataFlags::SeparateData | ERedUEBulkDataFlags::StoreInSeparateFile))
    {
        Ar.Seek(BulkDataOffsetInFile);
        SerializeDataChunk(Ar);
        ensure (BulkDataOffsetInFile + BulkDataSizeOnDisk == Ar.Tell());
    }
    else
    {
        SerializeDataChunk(Ar);
    }
}

void FRedUELegacyByteBulkData::Serialize(FRedUELegacyArchive& Ar)
{
    SerializeHeader(Ar);
    
    if (BulkDataFlags & ERedUEBulkDataFlags::Unused || ElementCount == 0)	// skip serializing
    {
        return;
    }
    if (BulkDataFlags & ERedUEBulkDataFlags::StoreInSeparateFile)
    {
        return;
    }
    if (BulkDataFlags & ERedUEBulkDataFlags::SeparateData)
    {
        // stored in the same file, but at different position
        // save archive position
        const int64 savePos = Ar.Tell();
        // seek to data block and read data
        Ar.PushStopper();
        Ar.SetStopper(INDEX_NONE);
        SerializeData(Ar);
        Ar.Seek(savePos);
        Ar.PopStopper();
        return;
    }
    if (ElementCount > 0)
    {
        SerializeData(Ar);
    }
}

void FRedUELegacyByteBulkData::Skip(FRedUELegacyArchive& Ar)
{
    SerializeHeader(Ar);
    if (BulkDataFlags & ERedUEBulkDataFlags::Unused || ElementCount == 0)	// skip serializing
    {
        return;
    }
    if (BulkDataOffsetInFile == Ar.Tell())
    {
        Ar.Seek(Ar.Tell() + BulkDataSizeOnDisk);
    }
}

void FRedUELegacyByteBulkData::LoadFromFile(const FString& Name)
{
	URedUELegacySubsystem*RedUELegacySubsystem = GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
	for (const FString&InContentPath :RedUELegacySubsystem->InContentPaths)
	{
		FString Path = FPaths::Combine( InContentPath,Name);
		if (!FPaths::FileExists(Path))
		{
			Path +=  TEXT(".tfc");
			if (!FPaths::FileExists(Path))
			{
				continue;
			}
		}
		if(FArchive *TFCReader = IFileManager::Get().CreateFileReader(*Path);ensure(TFCReader))
		{
			SerializeData(*TFCReader);
			delete TFCReader;
		}
		return;
	}
 
}

void FRedUELegacyByteBulkData::SerializeDataChunk(FArchive& Ar)
{
	ReleaseData();
	int32 DataSize = ElementCount * GetElementSize();
	if (!DataSize)
		return;
	BulkData = static_cast<uint8*>(FMemory::Malloc(DataSize));
	if (BulkDataFlags & (0x02 | 0x10 | 0x80))
	{
		int32 flags = 0;
		if (BulkDataFlags & 0x02) flags = 0x1;
		if (BulkDataFlags & 0x10)  flags = 0x2;
		if (BulkDataFlags & 0x80)  flags = 0x4;
		FRedUELegacyArchiveFileHandle::Decompress(Ar, BulkData, DataSize, flags);
	}
	else
	{
		Ar.Serialize(BulkData, DataSize);
	}
}

ULegacyPackage::ULegacyPackage()
{
	bIsLoaded = false;
	FileHandle = nullptr;
}

bool ULegacyPackage::LoadPackage(const TCHAR* InFileName)
{
	check(bIsLoaded == false);
    FileName = InFileName;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	URedUELegacySubsystem*RedUELegacySubsystem =  GetTypedOuter<URedUELegacySubsystem>();
	
	for (const FString&InContentPath :RedUELegacySubsystem->InContentPaths)
	{
		const FString Path = FPaths::Combine( InContentPath,FileName);
		if (FPaths::FileExists(Path))
		{
			FileHandle =  PlatformFile.OpenRead(*Path);
			break;
		}
	}
	
	if(!FileHandle)
	{
		FileHandle =  PlatformFile.OpenRead(*FileName);
	}
	if(!FileHandle)
	{
		return false;
	}
	{
		FileHandle->Seek(0);
		uint32 Tag, Version;
		*this << Tag;
		*this << Version;
		FileHandle->Seek(0);
		
		if (Version == PACKAGE_FILE_TAG || Version == 0x20000 || Version == 0x10000)
		{
			FRedUELegacyCompressedChunkHeader H;
			*this << H;
			TArray<FRedUELegacyCompressedChunk> Chunks;
			FRedUELegacyCompressedChunk &Chunk = Chunks.AddDefaulted_GetRef();
			Chunk.UncompressedOffset = 0;
			Chunk.UncompressedSize   = H.Sum.UncompressedSize;
			Chunk.CompressedOffset   = 0;
			Chunk.CompressedSize     = H.Sum.CompressedSize;
			FileHandle =  new FRedUELegacyArchiveFileHandle(FileHandle, 0xFF, Chunks);
		}
		
	}
	SetIsLoading(true);
	SetIsPersistent(true);
	if(!Summary.Serialize(*this))
	{
		return false;
	}

    switch (Game)
    {
    case ERedUELegacyGame::Bioshock:
        GameType = ERedUELegacyGameType::Bioshock;
        break;
    case ERedUELegacyGame::Singularity:
        GameType = ERedUELegacyGameType::Singularity;
        break;
	case ERedUELegacyGame::Bioshock3:
		GameType = ERedUELegacyGameType::Bioshock3;
    	break;
    default: 
        GameType = ERedUELegacyGameType::Unkown;
        break;
    }

    switch (Game&ERedUELegacyGame::Engine)
    {
	case ERedUELegacyGame::UE1:
	    EngineType = ERedUELegacyEngineType::UnrealEngine1;
        break;
	case ERedUELegacyGame::UE2:
	    EngineType = ERedUELegacyEngineType::UnrealEngine2;
        break;
	case ERedUELegacyGame::VENGEANCE:
	    EngineType = ERedUELegacyEngineType::Vengeance;
        break;
	case ERedUELegacyGame::UE2X:
	    EngineType = ERedUELegacyEngineType::UnrealEngine2X;
        break;
	case ERedUELegacyGame::UE3:
	    EngineType = ERedUELegacyEngineType::UnrealEngine3;
        break;
    default:
        checkNoEntry();
    }
    
    if (Game >= ERedUELegacyGame::UE3 && Summary.CompressionFlags && Summary.CompressedChunks.Num())
    {
        FileHandle =  new FRedUELegacyArchiveFileHandle(FileHandle, Summary.CompressionFlags, Summary.CompressedChunks);
    }

    LoadNameTable();
    LoadImportTable();
    LoadExportTable();
    bIsLoaded = true;
	PostLoadPackage();
	return true;
}

void ULegacyPackage::ClosePackage()
{
	check((!!FileHandle) == bIsLoaded);
	if(FileHandle)
	{
		delete FileHandle;
		FileHandle = nullptr;
	}
}

bool ULegacyPackage::AtEnd()
{
	return FileHandle->Tell()==FileHandle->Size();
}

FArchive& ULegacyPackage::operator<<(FName& Value)
{
	int32 N_Index = 0, N_ExtraIndex = 0;
    if (Game == ERedUELegacyGame::Bioshock)
    {
        *this << AR_INDEX(N_Index) << N_ExtraIndex;
        if (N_ExtraIndex == 0)
        {
            Value = *NameTable[N_Index];
        }
        else
        {
            Value = FName(FName(NameTable[N_Index]),N_ExtraIndex);
        }
        return *this;
    }
    if (Game == ERedUELegacyGame::UE2X && LegacyVer >= 145)
    {
        *this << N_Index;
    }
    else if (Game >= ERedUELegacyGame::UE3)
    {
        *this << N_Index;
        if (LegacyVer >= 343)
        {
           *this << N_ExtraIndex;
        }
    }
    else
    {
        *this << AR_INDEX(N_Index);
    }
    if (N_ExtraIndex == 0)
    {
        Value = *NameTable[N_Index];
    }
    else
    {
        Value = FName(FName(NameTable[N_Index]),N_ExtraIndex);
    }
	return *this;
}

FArchive& ULegacyPackage::operator<<(UObject*& Value)
{
    int32 Index;
    if (Game >= ERedUELegacyGame::UE3)
        *this << Index;
    else
        *this << AR_INDEX(Index);
    
    Value = nullptr;
    if (Index < 0)
    {
    	Value = GetOrCreateImport(-Index-1);
    }
    else if (Index > 0)
    {
        //		const FObjectExport &Exp = GetExport(index-1);
        //		appPrintf("PKG: Export[%d] OBJ=%s CLS=%s\n", index, *Exp.ObjectName, GetClassNameFor(Exp));
        Value = GetOrCreateExport(Index-1);
    }
	if (Value)
	{
		if (Value->IsA<ULegacyUnknown>())
		{
			Value = nullptr;
		}
	}
    return *this;
}

void ULegacyPackage::Seek(int64 InPos)
{
	FileHandle->Seek(InPos);
}

int64 ULegacyPackage::Tell()
{
	return FileHandle->Tell();
}

int64 ULegacyPackage::TotalSize()
{
	return FileHandle->Size();
}

void ULegacyPackage::Serialize(void* V, int64 Length)
{
    check(Stopper == INDEX_NONE||Stopper>=Tell()+Length);
	FileHandle->Read(static_cast<uint8*>(V),Length);
}

int32 ULegacyPackage::GetStopper()
{
    if(Stopper!=INDEX_NONE)
    {
        return Stopper;
    }
    return FRedUELegacyArchive::GetStopper();
}

void ULegacyPackage::SetStopper(int32 Pos)
{
    Stopper = Pos;
}

void ULegacyPackage::PushStopper()
{
    StoppersSaved.Push(Stopper);
}

void ULegacyPackage::PopStopper()
{
    Stopper = StoppersSaved.Pop();
}

void ULegacyPackage::LegacySerialize(FRedUELegacyArchive& Ar)
{
	
}

UObject* ULegacyPackage::ExportToContent()
{
	for (ULegacyObject*Object:Objects)
	{
		if (Object)
		{
			Object->ExportToContent();
		}
	}
	return nullptr;
}

UClass* ULegacyPackage::FindNearestClass(int32 Index)
{
	if (Index == 0)
	{
		return ULegacyClass::StaticClass();
	}
	
	FName PackageName = GetPackageName(Index);
	FName ObjectName = GetObjectName(Index);
	URedUELegacySubsystem*RedUELegacySubsystem =  GetTypedOuter<URedUELegacySubsystem>();
	if (FRedUELegacyVirtualPackage* VirtualPackage = RedUELegacySubsystem->VirtualPackages.Find(PackageName))
	{
		if (TSubclassOf<ULegacyObject>* FoundClass = VirtualPackage->Classes.Find(ObjectName))
		{
			return FoundClass->Get();
		}
	}
	
	ULegacyClass* LegacyClass;
	if (Index < 0)
	{
		LegacyClass = CastChecked<ULegacyClass>(GetOrCreateImport(-Index - 1),ECastCheckedType::NullAllowed);
	}
	else
	{
		LegacyClass = CastChecked<ULegacyClass>(GetOrCreateExport(Index - 1),ECastCheckedType::NullAllowed);
	}
	if (!LegacyClass)
	{
		if(!RedUELegacySubsystem->CacheNoFoundClasses.Contains(ObjectName))
		{
			RedUELegacySubsystem->CacheNoFoundClasses.Add(ObjectName);
			FMessageLog RedUELegacyMessageLog("RedUELegacy");
			RedUELegacyMessageLog.Error(FText::FromString(FString::Printf(TEXT("Can't found UClass %s"),*ObjectName.ToString())));
			RedUELegacyMessageLog.Open(EMessageSeverity::Error);
		}
		return nullptr;
	}
	RedUELegacySubsystem->ObjectPreload(LegacyClass);
	return FindNearestClass(LegacyClass);
}

UClass* ULegacyPackage::FindNearestClass(ULegacyClass* LegacyClass)
{
	URedUELegacySubsystem*RedUELegacySubsystem =  GetTypedOuter<URedUELegacySubsystem>();
	FName ClassName = LegacyClass->GetLegacyFName();
	
	if (TSubclassOf<ULegacyObject>* InClass = RedUELegacySubsystem->Classes.Find(ClassName))
	{
		return InClass->Get();
	}
	
	while (const FName* SubClass = GetDefault<URedUELegacyImporterSettings>()->EmptyClasses.Find(ClassName))
	{
		ClassName = *SubClass;
	}
	
	if (TSubclassOf<ULegacyObject>* InClass = RedUELegacySubsystem->Classes.Find(ClassName))
	{
		return InClass->Get();
	}
	
	if(!RedUELegacySubsystem->CacheNoFoundClasses.Contains(ClassName))
	{
		RedUELegacySubsystem->CacheNoFoundClasses.Add(ClassName);
		FMessageLog RedUELegacyMessageLog("RedUELegacy");
		RedUELegacyMessageLog.Error(FText::FromString(FString::Printf(TEXT("Can't found class %s"),*ClassName.ToString())));
		RedUELegacyMessageLog.Open(EMessageSeverity::Error);
	}
	
	ULegacyClass* SuperClass = CastChecked<ULegacyClass>(LegacyClass->SuperField);
	RedUELegacySubsystem->ObjectPreload(SuperClass);
	FName SuperClassName = SuperClass->GetLegacyFName();
	UClass* Result = FindNearestClass(SuperClass);
	for (TSoftClassPtr<ULegacyObject> IgnoreClass : GetDefault<URedUELegacyImporterSettings>()->IgnoredClassesForAutoMakeEmptyClass)
	{
		if (UClass* SomeBase = IgnoreClass.Get())
		{
			if (Result->IsChildOf(SomeBase))
			{
				return nullptr;
			}
		}
		
	}
	if (GetDefault<URedUELegacyImporterSettings>()->bAutoMakeEmptyClass && Result)
	{
		URedUELegacyImporterSettings* RedUELegacyImporterSettings  = GetMutableDefault<URedUELegacyImporterSettings>();
		RedUELegacyImporterSettings->PreEditChange(nullptr);
		RedUELegacyImporterSettings->EmptyClasses.Add(ClassName,SuperClassName);
		RedUELegacyImporterSettings->PostEditChange();
		RedUELegacyImporterSettings->TryUpdateDefaultConfigFile();
	}
    	
	return Result;
}

ULegacyObject* ULegacyPackage::GetOrCreateImport(int32 Index)
{
	auto CompareObjectPaths = [](ULegacyPackage* Package,int32 PackageIndex, ULegacyPackage *RefPackage, int32 RefPackageIndex)
	{
		while (PackageIndex || RefPackageIndex)
		{
			FName PackageName, RefPackageName;

			if (PackageIndex < 0)
			{
				const FRedUELegacyObjectImport &Rec = Package->GetImport(-PackageIndex-1);
				PackageIndex = Rec.PackageIndex;
				PackageName  = Rec.ObjectName;
			}
			else if (PackageIndex > 0)
			{
				const FRedUELegacyObjectExport &Rec = Package->GetExport(PackageIndex-1);
				PackageIndex = Rec.PackageIndex;
				PackageName  = Rec.ObjectName;
			}
			else
			{
				PackageName = *FPaths::GetBaseFilename(Package->FileName);
			}

			if (RefPackageIndex < 0)
			{
				const FRedUELegacyObjectImport &Rec = RefPackage->GetImport(-RefPackageIndex-1);
				RefPackageIndex = Rec.PackageIndex;
				RefPackageName  = Rec.ObjectName;
			}
			else if (RefPackageIndex > 0)
			{
				// possible for UE3 forced exports
				const FRedUELegacyObjectExport &Rec = RefPackage->GetExport(RefPackageIndex-1);
				RefPackageIndex = Rec.PackageIndex;
				RefPackageName  = Rec.ObjectName;
			}
			else
			{
				RefPackageName = *FPaths::GetBaseFilename(RefPackage->FileName);
			}
			FString LocRefPackageName = RefPackageName.ToString() + TEXT("_") + GetDefault<URedUELegacyImporterSettings>()->Language.ToUpper();
			if (RefPackageName != PackageName && LocRefPackageName != PackageName.ToString())
			{
				return false;
			}
		}

		return true;
	};
	auto FindExport = [](ULegacyPackage* Package,FName ObjectName, FName ClassName,int32 FirstIndex)->int32
	{
		for (int32 i = FirstIndex; i < Package->Summary.ExportCount; i++)
		{
			const FRedUELegacyObjectExport &Exp = Package->ExportTable[i];
			if (Exp.ObjectName == ObjectName)
			{
				if (ClassName  != Package->GetObjectName(Exp.ClassIndex))
					continue;
				return i;
			}
		}
		return INDEX_NONE;
	};
	auto FindExportForImport = [Index,FindExport,this,CompareObjectPaths](ULegacyPackage* Package,FName ObjectName, FName ClassName)->int32
	{
		int32 ObjIndex = -1;
		while (true)
		{
			ObjIndex = FindExport(Package,ObjectName, ClassName, ObjIndex + 1);
			if (ObjIndex == INDEX_NONE)
				break;
			
			if (CompareObjectPaths(Package,ObjIndex + 1, this, -1-Index))
				return ObjIndex;	
		}

		return INDEX_NONE;		
	};
	
	URedUELegacySubsystem*RedUELegacySubsystem =  GetTypedOuter<URedUELegacySubsystem>();
	FRedUELegacyObjectImport &ObjectImport = GetImport(Index);
	if(ObjectImport.Missing)
	{
		return nullptr;
	}
	if(ObjectImport.Object)
	{
		return ObjectImport.Object;
	}
	FName PackageName = GetObjectPackageName(ObjectImport.PackageIndex);
	ULegacyPackage* Package = nullptr;
	int32 ObjectIndex =  INDEX_NONE;

	if(PackageName != NAME_None)
	{
		Package = RedUELegacySubsystem->GetPackage(PackageName.ToString());
	}
	
	if(Package)
	{
		ObjectIndex = FindExportForImport(Package,ObjectImport.ObjectName, ObjectImport.ClassName);
	}
	else
	{
		Package = RedUELegacySubsystem->GetPackage(TEXT("Startup_XXX"));
		if (Package)
		{
			ObjectIndex = FindExportForImport(Package,ObjectImport.ObjectName, ObjectImport.ClassName);
		}
		if (ObjectIndex == INDEX_NONE)
		{
			for(auto&[Key,Value]:RedUELegacySubsystem->Packages)
			{
				Package = Value;
				ObjectIndex = FindExportForImport(Package,ObjectImport.ObjectName, ObjectImport.ClassName);
				if (ObjectIndex != INDEX_NONE)
				{
					break;
				}
			}
			
		}
	}
	
	if(ObjectIndex == INDEX_NONE)
	{
		ObjectImport.Missing = true;
		return nullptr;
	}
	
	return Package->GetOrCreateExport(ObjectIndex);
}

ULegacyObject* ULegacyPackage::GetOrCreateExport(int32 Index)
{
    FRedUELegacyObjectExport &Exp = GetExport(Index);
    if (Exp.Object)
    {
        return Exp.Object;
    }

    URedUELegacySubsystem*RedUELegacySubsystem =  GetTypedOuter<URedUELegacySubsystem>();
	
	ULegacyObject* SuperObject = nullptr;
	if (Exp.SuperIndex != 0)
	{
		if (Exp.SuperIndex < 0)
		{
			SuperObject = GetOrCreateImport(-Exp.SuperIndex-1);
		}
		else if (Exp.ClassIndex > 0)
		{
			SuperObject = GetOrCreateExport(Exp.SuperIndex-1);
		}
		RedUELegacySubsystem->ObjectPreload(SuperObject);
	}
	if (!SuperObject && Exp.PackageIndex != 0)
	{
		if (Exp.PackageIndex < 0)
		{
			SuperObject = GetOrCreateImport(-Exp.PackageIndex-1);
		}
		else if (Exp.PackageIndex > 0)
		{
			SuperObject = GetOrCreateExport(Exp.PackageIndex-1);
		}
		RedUELegacySubsystem->ObjectPreload(SuperObject);
	}
	else
	{
		SuperObject = this;
	}
	
	EObjectFlags OutObjectFlags = RF_NoFlags;
	uint64 InObjectFlags = Exp.ObjectFlags | static_cast<uint64>(Exp.ObjectFlags2) << 32ull;
	if (InObjectFlags & RLF_ClassDefaultObject || InObjectFlags & RLF_ArchetypeObject)
	{
		OutObjectFlags |= RF_ArchetypeObject;
		if (SuperObject && SuperObject->HasAnyFlags(RF_DefaultSubObject | RF_ArchetypeObject))
		{
			OutObjectFlags |= RF_DefaultSubObject;
		}
	}
	
	const FName ClassName = GetObjectName(Exp.ClassIndex);
	if (RedUELegacySubsystem->SequenceActionClasses.Contains(ClassName) && !RedUELegacySubsystem->Classes.Contains(ClassName))
	{
		ULegacySequenceImporter* SequenceImporter = NewObject<ULegacySequenceImporter>(SuperObject,ULegacySequenceImporter::StaticClass(),Exp.ObjectName,OutObjectFlags);
		if (ensure(SequenceImporter))
		{
			static FName  NAME_Action = "Action";
			SequenceImporter->ToAction = NewObject<USequenceAction>(SequenceImporter,RedUELegacySubsystem->SequenceActionClasses[ClassName],NAME_Action, OutObjectFlags|RF_DefaultSubObject);
			Exp.Object = SequenceImporter;
		}
	}
	else
	{
		if (UClass* Class = FindNearestClass(Exp.ClassIndex))
		{
			Exp.Object = NewObject<ULegacyObject>(SuperObject,Class,Exp.ObjectName,OutObjectFlags);
		}
	}

	if(!Exp.Object)
    {
    	Exp.Object = NewObject<ULegacyUnknown>(SuperObject, Exp.ObjectName, OutObjectFlags);
    }
	
	if (ULegacyPackage* SuperPackage = Cast<ULegacyPackage>(SuperObject))
	{
		SuperPackage->Objects.Add(Exp.Object);
	}
    {
    	ULegacyObject* ClassObject = nullptr;
    	if (Exp.ClassIndex != 0)
    	{
    		if (Exp.ClassIndex < 0)
    		{
    			ClassObject = GetOrCreateImport(-Exp.ClassIndex-1);
    		}
    		else if (Exp.ClassIndex > 0)
    		{
    			ClassObject = GetOrCreateExport(Exp.ClassIndex-1);
    		}
    	}
    	TFunction<void(ULegacyClass* ClassObject)> CopyFromDefaultObjects = [&CopyFromDefaultObjects,RedUELegacySubsystem,Object = Exp.Object](ULegacyClass* ClassObject)
    	{
    		if (!ClassObject)
    		{
    			return;
    		}
    		
    		RedUELegacySubsystem->ObjectPreload(ClassObject);
    		if (ULegacyClass* SuperClassObject = Cast<ULegacyClass>(ClassObject->SuperField))
    		{
    			CopyFromDefaultObjects(SuperClassObject);
    		}
    		if (ClassObject->ClassDefaultObject)
    		{
    			UEngine::FCopyPropertiesForUnrelatedObjectsParams Options;
    			Options.bDoDelta = false;
    			UEngine::CopyPropertiesForUnrelatedObjects(ClassObject->ClassDefaultObject, Object, Options);
    		}
    	};
    	CopyFromDefaultObjects(Cast<ULegacyClass>(ClassObject));
    	
    	// if (ULegacyClass* LegacyClass = Cast<ULegacyClass>(ClassObject); LegacyClass && LegacyClass->ClassDefaultObject)
    	// {
    	// 	UEngine::FCopyPropertiesForUnrelatedObjectsParams Options;
    	// 	Options.bDoDelta = false;
    	// 	UEngine::CopyPropertiesForUnrelatedObjects(LegacyClass->ClassDefaultObject, Exp.Object, Options);
    	// }
    }
    {
    	ULegacyObject* Template = nullptr;
    	if (Exp.Archetype != 0)
    	{
    		if (Exp.Archetype < 0)
    		{
    			Template = GetOrCreateImport(-Exp.Archetype-1);
    		}
    		else if (Exp.Archetype > 0)
    		{
    			Template = GetOrCreateExport(Exp.Archetype-1);
    		}
    		RedUELegacySubsystem->ObjectPreload(Template);
    	}
    	if (Template)
    	{
    		UEngine::FCopyPropertiesForUnrelatedObjectsParams Options;
    		Options.bDoDelta = false;
    		UEngine::CopyPropertiesForUnrelatedObjects(Template, Exp.Object, Options);
    	}
    }
	
    RedUELegacySubsystem->ObjectsBeginLoad();
    Exp.Object->LegacyPackage = this;
    Exp.Object->LegacyPackageIndex = Index;
    Exp.Object->LegacyObjectFlags = InObjectFlags;
	Exp.Object->LegacyObjectFlags |= RLF_NeedLoad;
    RedUELegacySubsystem->ObjectsLoaded.Add(Exp.Object);
    RedUELegacySubsystem->ObjectsEndLoad();
    return Exp.Object;
}

FName  ULegacyPackage::GetObjectPackageName(int PackageIndex) const
{
	FName PackageName = NAME_None;
	while (PackageIndex)
	{
		if (PackageIndex < 0)
		{
			const FRedUELegacyObjectImport &Rec = GetImport(-PackageIndex-1);
			PackageIndex = Rec.PackageIndex;
			PackageName  = Rec.ObjectName;
		}
		else
		{
			// possible for UE3 forced exports
			const FRedUELegacyObjectExport &Rec = GetExport(PackageIndex-1);
			PackageIndex = Rec.PackageIndex;
			PackageName  = Rec.ObjectName;
		}
	}
	return PackageName;
}

int32 ULegacyPackage::FindExport(FName Name)
{
    for(int32 Index = 0;Index<ExportTable.Num();Index++)
    {
        if(ExportTable[Index].ObjectName == Name)
        {
            return Index;
        }
    }
    return INDEX_NONE;
}

FString ULegacyPackage::GetFullExportName(int32 Index)
{
    TArray<FString> PackageNames;
    const FRedUELegacyObjectExport &Exp = GetExport(Index);
    PackageNames.Add(Exp.ObjectName.ToString());
    int32 CurrentPackageIndex = Exp.PackageIndex;
    while (CurrentPackageIndex)
    {
        FString PackageName;
        if (CurrentPackageIndex < 0)
        {
            const FRedUELegacyObjectImport &Rec = GetImport(-CurrentPackageIndex-1);
            CurrentPackageIndex = Rec.PackageIndex;
            PackageName = Rec.ObjectName.ToString();
        }
        else
        {
            const FRedUELegacyObjectExport &Rec = GetExport(CurrentPackageIndex-1);
            CurrentPackageIndex = Rec.PackageIndex;
            PackageName = Rec.ObjectName.ToString();
            // if (PackageIndex == 0 && (Rec.ExportFlags && EF_ForcedExport) && !IncludeCookedPackageName)
            //     break;		// do not add cooked package name
        }
        PackageNames.Add(PackageName);
    }
    FString Result;
    while(PackageNames.Num())
    {
        if(Result.Len()>0)
        {
            Result.Append(TEXT("."));
        }
        Result.Append(PackageNames.Pop());
    }
    return Result;
}

void ULegacyPackage::SetupReader(int32 ExportIndex)
{
	const FRedUELegacyObjectExport &Exp = GetExport(ExportIndex);
    Seek(Exp.SerialOffset);
    Stopper = Exp.SerialOffset+Exp.SerialSize;
}

ULegacyObject* ULegacyPackage::FindOrCreateExport(FName Name)
{
	int32 Index = FindExport(Name);
	if(Index!=INDEX_NONE)
	{
		return GetOrCreateExport(Index);
	}
	return nullptr;
}

void ULegacyPackage::LoadNameTable()
{
    Seek(Summary.NameOffset);
    for (int32 i = 0; i < Summary.NameCount; i++)
    {
        FString InName;
        *this<<InName;
        NameTable.Add(*InName);
        if ((Game >= ERedUELegacyGame::UE3 && LegacyVer >= 195)||Game == ERedUELegacyGame::Bioshock)
        {
            int64 flags64;
            *this << flags64;
        }
        else
        {
            int32 flags32;
            *this << flags32;
        }
    }
}

void ULegacyPackage::LoadImportTable()
{
	Seek(Summary.ImportOffset);
    for (int32 i = 0; i < Summary.ImportCount; i++)
    {
        ImportTable.AddDefaulted_GetRef().Serialize(*this);
    }
}

void ULegacyPackage::LoadExportTable()
{
    for (int32 i = 0; i < Summary.ExportCount; i++)
    {
        *this << ExportTable.AddDefaulted_GetRef();
    }
}

void ULegacyPackage::BeginDestroy()
{
	if(FileHandle != nullptr)
	{
	    ClosePackage();
	}
    Super::BeginDestroy();
}

void ULegacyPackage::PostLoadPackage()
{
}


FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FRedUELegacyObjectExport &E)
{
	if (Ar.Game >= ERedUELegacyGame::UE2)
	{
		E.Serialize3(Ar);
		return Ar;
	}
	if (EnumHasAllFlags(Ar.Game&ERedUELegacyGame::Engine,ERedUELegacyGame::UE2X))
	{
		E.Serialize2X(Ar);
		return Ar;
	}
	E.Serialize2(Ar);
	return Ar;
}
