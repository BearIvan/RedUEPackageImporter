#include "WWiseBanksManager.h"

#include "EdGraph/EdGraphSchema.h"
#include "Sound/SoundNodeDialoguePlayer.h"
#include "Sound/SoundNodeRandom.h"
#include "Sound/SoundNodeWavePlayer.h"
#include "SoundCueGraph/SoundCueGraphNode.h"

THIRD_PARTY_INCLUDES_START

extern "C"
{
#include <ogg/os_types.h>
#include <vorbis/vorbisfile.h>
}

#include "Windows/AllowWindowsPlatformTypes.h"
#include "ww2ogg/wwriff.h"
#include "Windows/HideWindowsPlatformTypes.h"

THIRD_PARTY_INCLUDES_END

#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/RedUELegacySubsystem.h"
#include "HAL/FileManagerGeneric.h"
#include "Interfaces/IPluginManager.h"

USoundBase* FWWiseBankObjectBase::ExportToContent()
{
	
	ensure(false);
	return nullptr;
}

void FWWiseBankObjectUnknown::Serialize(FArchive& Ar, int32 Size)
{
	Data.AddUninitialized(Size);
	Ar.Serialize(Data.GetData(), Size);
}

void FWWiseBankObjectSoundEffectOrVoice::Serialize(FArchive& Ar, int32 Size)
{
	int32 Unknown0;
	int32 Unknown1;
	Ar<<Unknown0<<Unknown1<<ObjectID;
	Ar.Seek(Ar.Tell() + Size - 12);
	
}


int ov_seek_func(void* datasource, ogg_int64_t offset, int whence)
{
	switch (whence)
	{
	case SEEK_SET:
		((FMemoryReader*)datasource)->Seek((int)offset);
		break;
	case SEEK_CUR:
		((FMemoryReader*)datasource)->Seek((int)offset+ ((FArchive*)datasource)->Tell());
		break;
	case SEEK_END:
		((FMemoryReader*)datasource)->Seek((int)offset + ((FArchive*)datasource)->TotalSize());
		break;
	}
	return 0;
}
size_t ov_read_func(void* ptr, size_t size, size_t nmemb, void* datasource)
{
	FMemoryReader* F = (FMemoryReader*)datasource;
	size_t elapsed = F->TotalSize()-F->Tell();
	size_t exist_block = max(static_cast<size_t>(0), static_cast<size_t>(elapsed / size));
	size_t read_block = min(exist_block, nmemb);
	F->Serialize(ptr, (int)(read_block * size));
	return read_block;
}

int ov_close_func(void* datasource)
{
	return 0;
}

long ov_tell_func(void* datasource)
{
	return ((FMemoryReader*)datasource)->Tell();
}

USoundBase* FWWiseBankObjectSoundEffectOrVoice::ExportToContent()
{
	return OwnerBank->CreateOrLoadSound(ObjectID);
}

bool FWWiseBankObjectSoundEffectOrVoice::CanCreateInCue() const
{
	if (OwnerBank->IsValidSound(ObjectID))
	{
		return true;
	}
	return false;
}

void FWWiseBankObjectEvent::Serialize(FArchive& Ar, int32 Size)
{
	if (OwnerBank->VersionBank >= 134)
	{	
		uint8 Count = 0;
		Ar << Count;
		ObjectIDs.AddUninitialized(Count);
	}
	else
	{
		uint32 Count = 0;
		Ar << Count;
		ObjectIDs.AddUninitialized(Count);
	}
	for (int32& Action : ObjectIDs)
	{
		Ar << Action;
	}
}

USoundBase* FWWiseBankObjectEvent::ExportToContent()
{
	URedUELegacySubsystem* RedUELegacySubsystem = OwnerBank->Owner->GetTypedOuter<URedUELegacySubsystem>();

	TArray<FWWiseBankObjectBase*> CanExportsToContent;
	for (int32&  ObjectID : ObjectIDs)
	{
		TUniquePtr<FWWiseBankObjectBase>* Object = OwnerBank->Objects.Find(ObjectID);
		if (ensure(Object))
		{
			if ((*Object)->CanCreateInCue())
			{
				CanExportsToContent.Add(Object->Get());
			}
		}
	}
	if (CanExportsToContent.Num() == 0)
	{
		return nullptr;
	}
	
	FString ObjectName = FString::Printf(TEXT("Event_%x"),EventID);
	const FString ObjectPath = RedUELegacySubsystem->OutContentPath / TEXT("Audio") / OwnerBank->BankName / ObjectName;
    const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
    const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
    USoundCue* SoundCue = LoadObject<USoundCue>(nullptr, *FullObjectPath,nullptr,LOAD_NoWarn);
    if(!SoundCue)
    {
    	UPackage*  AssetPackage = CreatePackage(*PackageName);
    	SoundCue = NewObject<USoundCue>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public|RF_Standalone);
    	SoundCue->PreEditChange(nullptr);
    	FAssetRegistryModule::AssetCreated(SoundCue);

    	TArray<USoundBase*> SoundObjects;
    	for (FWWiseBankObjectBase* Object : CanExportsToContent)
    	{
    		 if (USoundBase* InSoundObject = Object->ExportToContent())
    		 {
	    		 SoundObjects.AddUnique(InSoundObject);
    		 }
    	}

		
    	{

    		constexpr int32 InitialPosX = -200;
    		constexpr int32 InitialPosY = 0;
    		
    		auto CreateSoundPlayerNode = [](USoundCue* SoundCue, UObject* SoundObject, int32 NodePosX, int32 NodePosY)->USoundNode*
    		{
    			if (!SoundCue || !SoundObject)
    			{
    				return nullptr;
    			}

    			if (USoundWave* SoundWave = Cast<USoundWave>(SoundObject))
    			{
    				if (USoundNodeWavePlayer* WavePlayer = SoundCue->ConstructSoundNode<USoundNodeWavePlayer>())
    				{
    					WavePlayer->SetSoundWave(SoundWave);
    					WavePlayer->GraphNode->NodePosX = NodePosX - CastChecked<USoundCueGraphNode>(WavePlayer->GetGraphNode())->EstimateNodeWidth();
    					WavePlayer->GraphNode->NodePosY = NodePosY;
    					return WavePlayer;
    				}
    			}
    			else if (UDialogueWave* DialogueWave = Cast<UDialogueWave>(SoundObject))
    			{
    				if (USoundNodeDialoguePlayer* DialoguePlayer = SoundCue->ConstructSoundNode<USoundNodeDialoguePlayer>())
    				{
    					DialoguePlayer->SetDialogueWave(DialogueWave);
    					DialoguePlayer->GraphNode->NodePosX = NodePosX - CastChecked<USoundCueGraphNode>(DialoguePlayer->GetGraphNode())->EstimateNodeWidth();
    					DialoguePlayer->GraphNode->NodePosY = NodePosY;

    					if (DialogueWave->ContextMappings.Num() == 1)
    					{
    						DialoguePlayer->DialogueWaveParameter.Context.Speaker = DialogueWave->ContextMappings[0].Context.Speaker;
    						DialoguePlayer->DialogueWaveParameter.Context.Targets = DialogueWave->ContextMappings[0].Context.Targets;
    					}
    					return DialoguePlayer;
    				}
    			}

    			return nullptr;
    		};
    		
    		if (SoundObjects.Num() > 0)
    		{
    			if (SoundObjects.Num() == 1)
    			{
    				USoundNode* PlayerNode = CreateSoundPlayerNode(SoundCue, SoundObjects[0], InitialPosX, InitialPosY);
    				SoundCue->FirstNode = PlayerNode;
    				SoundCue->LinkGraphNodesFromSoundNodes();
    			}
    			else
    			{
    				int32 PosX = InitialPosX;
    				int32 PosY = InitialPosY;

    				USoundNodeRandom* RandomNode = nullptr;
    				{
    					RandomNode = SoundCue->ConstructSoundNode<USoundNodeRandom>();
    					SoundCue->FirstNode = RandomNode;
    					SoundCue->LinkGraphNodesFromSoundNodes();
    					RandomNode->GraphNode->NodePosX = PosX;
    					RandomNode->GraphNode->NodePosY = PosY;
    				}
    				PosX -= 300;

    				TArray<USoundNode*> PlayerNodes;
    				for (TWeakObjectPtr<UObject> SoundObject : SoundObjects)
    				{
    					if (USoundNode* PlayerNode = CreateSoundPlayerNode(SoundCue, SoundObject.Get(), PosX, PosY))
    					{
    						const int32 ChildNodeIndex = RandomNode->ChildNodes.Num(); 
    						if (ChildNodeIndex < RandomNode->GetMaxChildNodes())
    						{ 
    							RandomNode->InsertChildNode(ChildNodeIndex);
    							RandomNode->ChildNodes[ChildNodeIndex] = PlayerNode;
    						}

    						PlayerNodes.Add(PlayerNode);
    						PosY += 100;
    					}
    				}

    				ensure (SoundObjects.Num() > RandomNode->GetMaxChildNodes());

    				if (UEdGraph* SoundCueGraph = SoundCue->GetGraph())
    				{
    					if (const UEdGraphSchema* GraphSchema = Cast<UEdGraphSchema>(SoundCueGraph->GetSchema()))
    					{
    						for (int32 Index = 0; Index < PlayerNodes.Num(); Index++)
    						{
    							if ( Index < RandomNode->GetMaxChildNodes() )
    							{
    								USoundCueGraphNode* GraphNode = CastChecked<USoundCueGraphNode>(RandomNode->GetGraphNode());
    								if (Index >= GraphNode->GetInputCount())
    								{
    									GraphNode->CreateInputPin();
    								}
    								GraphSchema->TryCreateConnection(GraphNode->GetInputPin(Index), CastChecked<USoundCueGraphNode>(PlayerNodes[Index]->GetGraphNode())->GetOutputPin() );
    							}
    						}
    					}
    				}
    			}
    		}
    	}
    	SoundCue->PostEditChange();
    	SoundCue->Modify();
    	
    	
        
    }
	

	return SoundCue;
}

void FWWiseBankObjectEventAction::Serialize(FArchive& Ar, int32 Size)
{
	uint8 Pid1,ParameterCount;
	Ar<<Scope;			//1
	Ar<<ActionType;		//2
	Ar<<ObjectID;		//6
	Ar<<Pid1;			//7
	Ar<<ParameterCount;	//8
	
	// for (int32 i = 0; i < static_cast<int32>(ParameterCount); i++)
	// {
	// 	Ar << parameters_types.AddZeroed_GetRef();
	// }
	//
	// for (int32 i = 0; i < static_cast<int32>(ParameterCount); i++)
	// {
	// 	Ar << parameters.AddZeroed_GetRef();
	// }

	Ar.Seek(Size - 8 + Ar.Tell());
}

bool FWWiseBankObjectEventAction::CanCreateInCue() const
{
	if (ActionType == EWWiseEventActionType::Play)
	{
		TUniquePtr<FWWiseBankObjectBase>* Object = OwnerBank->Objects.Find(ObjectID);
		if (ensure(Object))
		{
			return (*Object)->CanCreateInCue();
		}
	}
	return false;
}

USoundBase* FWWiseBankObjectEventAction::ExportToContent()
{
	if (ActionType == EWWiseEventActionType::Play)
	{
		TUniquePtr<FWWiseBankObjectBase>* Object = OwnerBank->Objects.Find(ObjectID);
		if (ensure(Object))
		{
			return (*Object)->ExportToContent();
		}
	}
	return FWWiseBankObjectBase::ExportToContent();
}

FArchive& operator<<(FArchive& Ar, FWWiseBankSound& BankSound)
{
	Ar<<BankSound.Id;
	int32 Offset32;
	Ar<<Offset32;
	BankSound.Offset = Offset32;
	Ar<<BankSound.Size;
	return Ar;
}

FWWiseBank::FWWiseBank(class UWWiseBanksManager* InOwner):Owner(InOwner)
{
}

FWWiseBank::~FWWiseBank()
{
}

bool FWWiseBank::Load(const FString& InFileName)
{
	check(FileName.IsEmpty());
	FileName = InFileName;
	IFileHandle* FileHandle = FPlatformFileManager::Get().GetPlatformFile().OpenRead(*FileName);
	if (FileHandle == nullptr)
	{
		return false;
	}
	BankName = FPaths::GetCleanFilename(FileName);
	FArchiveFileReaderGeneric Ar(FileHandle,*FileName,FileHandle->Size());
	while (!Ar.AtEnd())
	{
		char ChunkName[4];
		uint32 ChunkLength;
		Ar << ChunkName[0] << ChunkName[1] << ChunkName[2] << ChunkName[3];
		Ar << ChunkLength;
		int32 ChunkDataOffset = Ar.Tell();
		if (FMemory::Memcmp(ChunkName,"BKHD",4) == 0)
		{
			Ar << VersionBank << BankId;
		}
		else if (FMemory::Memcmp(ChunkName,"AKPK",4) == 0)
		{
			DataOffset = 0;
			uint8 Unknown[0x2C];
			Ar.Serialize(Unknown,sizeof(Unknown));
			int32 Count;
			Ar << Count;
			for (int32 i = 0; i < Count; i++)
			{
				int32 Index;
				int32 Unknown2 =0;
				int64 Offset;
				int32 Size;
				Ar<<Index<<Unknown2<<Size<<Offset;
				ensure(Unknown2 == 1);
				
				FWWiseBankSound Sound;
				Sound.Offset = Offset;
				Sound.Size = Size;
				Sound.Id = Index;
				Sounds.Add(Sound.Id,Sound);
			}
		}
		else if (FMemory::Memcmp(ChunkName, "HIRC",4) == 0)
		{
			int32 Count;
			Ar << Count;
			
			for (int32 i = 0; i < Count; i++)
			{
				EWWiseBankObjectType ObjectType;
				Ar << ObjectType;
				int32 DataSize,ObjectId;
				Ar << DataSize << ObjectId;

				TUniquePtr<FWWiseBankObjectBase> Result;
				switch (ObjectType)
				{
					case EWWiseBankObjectType::Event:
						Result = MakeUnique<FWWiseBankObjectEvent>(this,ObjectId);
						break;
					case EWWiseBankObjectType::EventAction:
						Result = MakeUnique<FWWiseBankObjectEventAction>(this);
						break;
					case EWWiseBankObjectType::SoundEffectOrVoice:
						Result = MakeUnique<FWWiseBankObjectSoundEffectOrVoice>(this);
						break;
					default:
						Result = MakeUnique<FWWiseBankObjectUnknown>(this,ObjectType);
						break;
				}
				int32 CheckTellObject = Ar.Tell();
				Result->Serialize(Ar,DataSize - 4);
				if (!ensure(CheckTellObject + DataSize - 4 == Ar.Tell()))
				{
					Ar.Seek(CheckTellObject + DataSize - 4);
				}
				Objects.Add(ObjectId,MoveTemp(Result));
			}
		}
		else if (FMemory::Memcmp(ChunkName,"DATA",4) == 0)
		{
			DataOffset = Ar.Tell();
		}
		else if (FMemory::Memcmp(ChunkName,"DIDX",4) == 0)
		{
			int32 Count = ChunkLength/sizeof(FWWiseBankSound);
			for (int32 i = 0; i < Count; i++)
			{
				FWWiseBankSound Sound;
				Ar << Sound;
				Sounds.Add(Sound.Id,Sound);
			}
		}
		
		Ar.Seek(ChunkDataOffset + ChunkLength);
	}
	return true;
}

USoundBase* FWWiseBank::ExportToContent(int32 ObjectID)
{
	if (TUniquePtr<FWWiseBankObjectBase>* Object = Objects.Find(ObjectID);ensure(Object))
	{
		return (*Object)->ExportToContent();
	}
	return nullptr;
}

bool FWWiseBank::IsValid(int32 ObjectID) const
{
	return Objects.Contains(ObjectID);
}

bool FWWiseBank::IsValidSound(int32 SoundID) const
{
	if (Sounds.Contains(SoundID))
	{
		return true;
	}
	if (Owner)
	{
		return Owner->IsValidSoundFromGlobal(SoundID);
	}
	return false;
}

USoundBase* FWWiseBank::CreateOrLoadSound(int32 SoundID) const
{
	if (Sounds.Contains(SoundID))
	{
		URedUELegacySubsystem* RedUELegacySubsystem = Owner->GetTypedOuter<URedUELegacySubsystem>();
		FString ObjectName = FString::Printf(TEXT("Sound_%x"),SoundID);
		const FString ObjectPath = RedUELegacySubsystem->OutContentPath / TEXT("Audio") / BankName / TEXT("Sounds") / ObjectName;
		const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
		const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
		USoundWave* SoundWave = LoadObject<USoundWave>(nullptr, *FullObjectPath,nullptr,LOAD_NoWarn);
		if(!SoundWave)
		{
			if (const FWWiseBankSound* Sound = Sounds.Find(SoundID);ensure(Sound))
			{
				TArray<uint8> WWData;
				TArray<uint8> OGGData;
				{
					IFileHandle* FileHandle = FPlatformFileManager::Get().GetPlatformFile().OpenRead(*FileName);
					WWData.AddZeroed(Sound->Size);
					FileHandle->Seek(Sound->Offset + DataOffset);
					FileHandle->Read(WWData.GetData(), Sound->Size);
					delete FileHandle;
				}

				{

					FString PluginBaseDirectory = IPluginManager::Get().FindPlugin(TEXT("RedUEPackageImporter"))->GetBaseDir();
					std::string codebooks_filename;
					{
						codebooks_filename = TCHAR_TO_ANSI( *FPaths::Combine(PluginBaseDirectory, TEXT("Resources"),TEXT("packed_codebooks_aoTuV_603.bin")));
					}				

					FString SavedTemp = FPaths::ProjectSavedDir()/TEXT("Temp");
					const FString WWFile = FPaths::CreateTempFilename(*SavedTemp);
					const FString OGGFile = FPaths::CreateTempFilename(*SavedTemp);
					const FString OGGRevorbFile = FPaths::CreateTempFilename(*SavedTemp);
				
					if (!FFileHelper::SaveArrayToFile(WWData,*WWFile))
					{
						return nullptr;
					}
					try
					{
						std::ifstream in(TCHAR_TO_ANSI(*WWFile), ios::binary);
						Wwise_RIFF_Vorbis ww(in, codebooks_filename,  false,  false,   kNoForcePacketFormat );
						ofstream of(TCHAR_TO_ANSI(*OGGFile), ios::binary);
						ww.generate_ogg(of);
					}
					catch (...)
					{
						IFileManager::Get().Delete(*WWFile);
						IFileManager::Get().Delete(*OGGFile);
						return nullptr;
					}
				
					IFileManager::Get().Delete(*WWFile);
				
					const FString Params = FString::Printf(TEXT("%s %s"),*OGGFile,*OGGRevorbFile);
					FProcHandle ProcessHandle = FPlatformProcess::CreateProc(*FPaths::Combine(PluginBaseDirectory, TEXT("Resources"),TEXT("revorb.exe")),*Params, true, true, true,nullptr, 0, *SavedTemp, NULL, NULL);
					while (FPlatformProcess::IsProcRunning(ProcessHandle))
					{
						FPlatformProcess::Sleep(10);
					}
					FPlatformProcess::CloseProc(ProcessHandle);
					IFileManager::Get().Delete(*OGGFile);
					if (!FFileHelper::LoadFileToArray(OGGData,*OGGRevorbFile))
					{
						IFileManager::Get().Delete(*OGGRevorbFile);
						return nullptr;
					}
					IFileManager::Get().Delete(*OGGRevorbFile);
				}
				TArray<int16> RawData;
				int32 SampleRate = 0;
				int32 NumChannels = 0;
				float Duration = 0;
				{
					FMemoryReader Mem(OGGData);
					// Load file into memory and parse WAV-format
					OggVorbis_File ovf;
					ov_callbacks ovc = { ov_read_func, ov_seek_func, ov_close_func, ov_tell_func };
					ov_open_callbacks(&Mem, &ovf, nullptr, 0, ovc);

					vorbis_info* ovi = ov_info(&ovf, -1);
					SampleRate = ovi->rate;
					NumChannels = ovi->channels;
					ogg_int64_t pcm_total = ov_pcm_total(&ovf, -1)*2* ovi->channels;
					RawData.AddUninitialized(pcm_total/2);
					int current_section;
					long TotalRet = 0, ret;
					long left = pcm_total;
					Duration = static_cast<float>(pcm_total) / static_cast<float>(SampleRate* NumChannels*2);
					// Read loop
					while (TotalRet < (long)left)
					{
						ret = ov_read(&ovf, /*PCM*/ (char*)RawData.GetData() + TotalRet, left - TotalRet, 0, 2, 1, &current_section);

						// if end of file or read limit exceeded
						if (ret == 0)
							break;
						else if (ret < 0) 		// Error in bitstream
						{
							//
						}
						else
						{
							TotalRet += ret;
						}
					}

					ov_clear(&ovf);
				}
			
				UPackage*  AssetPackage = CreatePackage(*PackageName);
				SoundWave = NewObject<USoundWave>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public|RF_Standalone);
				SoundWave->PreEditChange(nullptr);
				FAssetRegistryModule::AssetCreated(SoundWave);
				{
					SoundWave->InvalidateCompressedData(true, false);

					TArray<uint8> RawWaveData;
					SerializeWaveFile(RawWaveData, (uint8*)RawData.GetData(), RawData.Num()*2, NumChannels, SampleRate);

					FSharedBuffer Buffer = FSharedBuffer::Clone(RawWaveData.GetData(), RawWaveData.Num());

					SoundWave->RawData.UpdatePayload(Buffer);

					SoundWave->RawPCMDataSize = RawData.Num();
					SoundWave->RawPCMData = (uint8*)FMemory::Malloc(SoundWave->RawPCMDataSize);
					FMemory::Memcpy(SoundWave->RawPCMData, RawData.GetData(), SoundWave->RawPCMDataSize);

					// Calculate the duration of the sound wave
					// Note: We use the NumInputChannels for duration calculation since NumChannelsToSerialize may be 1 channel while NumInputChannels is 2 for the "split stereo" feature.
					SoundWave->Duration = Duration;
					SoundWave->SetSampleRate(SampleRate);
					SoundWave->NumChannels = NumChannels;
					SoundWave->SetSoundAssetCompressionType(ESoundAssetCompressionType::BinkAudio);
				}
				SoundWave->PostEditChange();
				SoundWave->Modify();
			}
		}
		return SoundWave;
	}
	if (Owner)
	{
		return Owner->CreateOrLoadSoundFromGlobal(SoundID);
	}
	return nullptr;
}

USoundBase* UWWiseBanksManager::ExportToContent(const FString&LevelName, int32 AudioID)
{
	LoadGlobalBanks();
	
	TArray<TSharedPtr<FWWiseBank>>* Banks = WWiseBanks.Find(LevelName);
	if (!Banks)
	{
		Banks = LoadBanksForLevel(LevelName);
	}
	if (ensure(Banks))
	{
		for (TSharedPtr<FWWiseBank>& Bank : *Banks)
		{
			if (Bank->IsValid(AudioID))
			{
				return Bank->ExportToContent(AudioID);
			}
		}
	}
	return nullptr;
	
}

void UWWiseBanksManager::Empty()
{
	WWiseBanks.Empty();
	GlobalBanks.Empty();
}

bool UWWiseBanksManager::IsValidSoundFromGlobal(int32 SoundID) const
{
	for (const TSharedPtr<FWWiseBank>& Bank :GlobalBanks)
	{
		if (Bank->Sounds.Contains(SoundID))
		{
			return true;
		}
	}
	return false;
}

USoundBase* UWWiseBanksManager::CreateOrLoadSoundFromGlobal(int32 SoundID) const
{
	for (const TSharedPtr<FWWiseBank>& Bank :GlobalBanks)
	{
		if (Bank->Sounds.Contains(SoundID))
		{
			if (USoundBase* Sound = Bank->CreateOrLoadSound(SoundID))
			{
				return Sound;
			}
		}
	}
	return nullptr;
}

TArray<TSharedPtr<FWWiseBank>>* UWWiseBanksManager::LoadBanksForLevel(const FString&LevelName)
{
	TArray<TSharedPtr<FWWiseBank>> Result;
	for (const FString& ContentPath : GetTypedOuter<URedUELegacySubsystem>()->InContentPaths)
	{
		FString BanksPath = FPaths::Combine(ContentPath,TEXT(".."),TEXT("Audio"),TEXT("Packed"),TEXT("Windows"),TEXT("Banks"));

		FString FileName = FPaths::Combine(BanksPath,LevelName.ToUpper() + TEXT(".bnk"));
		if (FPaths::FileExists(FileName))
		{
			TSharedPtr<FWWiseBank> Bank = MakeShared<FWWiseBank>(this);
			if (Bank->Load(FileName))
			{
				Result.Add(Bank);
			}
		}
	}
	if (Result.Num() > 0)
	{
		return &WWiseBanks.Add(LevelName, Result);
	}
	return nullptr;
	
}

void UWWiseBanksManager::LoadGlobalBanks()
{
	if (GlobalBanks.Num() > 0)
	{
		return;
	}
	for (const FString& ContentPath : GetTypedOuter<URedUELegacySubsystem>()->InContentPaths)
	{
		FString BanksPath = FPaths::Combine(ContentPath,TEXT(".."),TEXT("Audio"),TEXT("Packed"),TEXT("Windows"),TEXT("Packs"));

		TArray<FString> Files;
		IFileManager::Get().FindFiles(Files,*BanksPath,TEXT("*.pck"));

		for (FString File : Files)
		{
			FString FilePath = FPaths::Combine(BanksPath,File);
			if (ensure(FPaths::FileExists(FilePath)))
			{
				TSharedPtr<FWWiseBank> Bank = MakeShared<FWWiseBank>(this);
				if (Bank->Load(FilePath))
				{
					GlobalBanks.Add(Bank);
				}
			}
		}
	
	}
}
