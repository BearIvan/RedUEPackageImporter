#include "Core\RedUELegacyPackage.h"

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
	if (Ar.LegacyVer >= 623)
		Ar << ImportExportGuidsOffset << ImportGuidsCount << ExportGuidsCount;
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

URedUELegacyPackage::URedUELegacyPackage()
{
	bIsLoaded = false;
	FileHandle = nullptr;
}

bool URedUELegacyPackage::LoadPackage(const TCHAR* FileName)
{
	check(bIsLoaded == false);
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FileHandle =  PlatformFile.OpenRead(FileName);
	if(!FileHandle)
	{
		return false;
	}
	SetIsLoading(true);
	SetIsPersistent(true);
	if(!Summary.Serialize(*this))
	{
		return false;
	}
	return true;
}

void URedUELegacyPackage::ClosePackage()
{
	check((!!FileHandle) == bIsLoaded);
	if(FileHandle)
	{
		delete FileHandle;
		FileHandle = nullptr;
	}
}

bool URedUELegacyPackage::AtEnd()
{
	return FileHandle->Tell()==FileHandle->Size();
}

FArchive& URedUELegacyPackage::operator<<(FName& Value)
{
	return *this;
}

FArchive& URedUELegacyPackage::operator<<(UObject*& Value)
{
	return FRedUELegacyArchive::operator<<(Value);
}

void URedUELegacyPackage::Seek(int64 InPos)
{
	FileHandle->Seek(InPos);
}

int64 URedUELegacyPackage::Tell()
{
	return FileHandle->Tell();
}

int64 URedUELegacyPackage::TotalSize()
{
	return FileHandle->Size();
}

void URedUELegacyPackage::Serialize(void* V, int64 Length)
{
	FileHandle->Read(static_cast<uint8*>(V),Length);
}

void URedUELegacyPackage::BeginDestroy()
{
	check(FileHandle == nullptr);
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
