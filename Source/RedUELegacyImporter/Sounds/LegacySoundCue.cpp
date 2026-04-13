#include "LegacySoundCue.h"

#include <vorbis/vorbisfile.h>

#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/RedUELegacyArchive.h"
#include "EdGraph/EdGraphSchema.h"
#include "Sound/SoundNodeAttenuation.h"
#include "Sound/SoundNodeConcatenator.h"
#include "Sound/SoundNodeLooping.h"
#include "Sound/SoundNodeWavePlayer.h"
#include "SoundCueGraph/SoundCueGraphNode.h"

USoundNode* ULegacySoundNode::Export(USoundCue* SoundCue)
{
	return nullptr;
}

void ULegacySoundNode::InsertNodes(USoundCue* SoundCue, USoundNode* To, const TArray<USoundNode*>& Nodes)
{
	TArray<USoundNode*> InsertNodes;
	
	for (USoundNode* Node : Nodes)
	{
		if (Node)
		{
			const int32 ChildNodeIndex = To->ChildNodes.Num(); 
			To->InsertChildNode(ChildNodeIndex);
			To->ChildNodes[ChildNodeIndex] = Node;
			InsertNodes.Add(Node);
		}
	}
	if (UEdGraph* SoundCueGraph = SoundCue->GetGraph())
	{
		if (const UEdGraphSchema* GraphSchema = Cast<UEdGraphSchema>(SoundCueGraph->GetSchema()))
		{
			for (int32 Index = 0; Index < InsertNodes.Num(); Index++)
			{
				if ( Index < To->GetMaxChildNodes() )
				{
					USoundCueGraphNode* GraphNode = CastChecked<USoundCueGraphNode>(To->GetGraphNode());
					if (Index >= GraphNode->GetInputCount())
					{
						GraphNode->CreateInputPin();
					}
					GraphSchema->TryCreateConnection(GraphNode->GetInputPin(Index), CastChecked<USoundCueGraphNode>(InsertNodes[Index]->GetGraphNode())->GetOutputPin() );
				}
			}
		}
	}
}

void ULegacySoundNode::InsertNode(USoundCue* SoundCue, USoundNode* To, USoundNode* Node)
{
	TArray<USoundNode*> Nodes;
	Nodes.Add(Node);
	InsertNodes(SoundCue, To, Nodes);
}

USoundNode* ULegacySoundNodeWave::Export(USoundCue* SoundCue)
{
	if (USoundNodeWavePlayer* WavePlayer = SoundCue->ConstructSoundNode<USoundNodeWavePlayer>())
	{
		WavePlayer->SetSoundWave(CastChecked<USoundWave>(ExportToContent(),ECastCheckedType::NullAllowed));
		return WavePlayer;
	}
	return nullptr;
}


int ov_seek_func(void* datasource, ogg_int64_t offset, int whence);
size_t ov_read_func(void* ptr, size_t size, size_t nmemb, void* datasource);
int ov_close_func(void* datasource);
long ov_tell_func(void* datasource);

UObject* ULegacySoundNodeWave::ExportToContent()
{
	USoundWave* SoundWave;
	if(TryLoadOrCreate(SoundWave))
	{
		FAssetRegistryModule::AssetCreated(SoundWave);
		SoundWave->Modify();
		SoundWave->PreEditChange(nullptr);
		TArray<int16> RawDataFromOGG;
		int32 SampleRate = 0;
		int32 NumChannels = 0;
		float Duration = 0;
		
	FRedUELegacyByteBulkData* OGGData = &RawData;
		
		FRedUELegacyByteBulkData* Bulks[] = {
			&CompressedPCData,
			&CompressedXbox360Data,
			&CompressedPS3Data,
			&CompressedWiiUData,		// appeared in ArVer 845
			&CompressedIPhoneData,		// appeared in ArVer 851
		};
	
		for (auto& Bulk : Bulks)
		{
			if (OGGData->ElementCount > 0) break;
			OGGData = Bulk;
		}
		if (OGGData && OGGData->ElementCount > 0)
		{
			FMemoryReaderView Mem(FMemoryView(OGGData->BulkData, OGGData->ElementCount));
			// Load file into memory and parse WAV-format
			OggVorbis_File ovf;
			ov_callbacks ovc = { ov_read_func, ov_seek_func, ov_close_func, ov_tell_func };
			ov_open_callbacks(&Mem, &ovf, nullptr, 0, ovc);

			vorbis_info* ovi = ov_info(&ovf, -1);
			SampleRate = ovi->rate;
			NumChannels = ovi->channels;
			ogg_int64_t pcm_total = ov_pcm_total(&ovf, -1)*2* ovi->channels;
			RawDataFromOGG.AddUninitialized(pcm_total/2);
			int current_section;
			long TotalRet = 0, ret;
			long left = pcm_total;
			Duration = static_cast<float>(pcm_total) / static_cast<float>(SampleRate* NumChannels*2);
			// Read loop
			while (TotalRet < (long)left)
			{
				ret = ov_read(&ovf, /*PCM*/ (char*)RawDataFromOGG.GetData() + TotalRet, left - TotalRet, 0, 2, 1, &current_section);

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
		if (RawDataFromOGG.Num() > 0)
		{
			SoundWave->InvalidateCompressedData(true, false);

			TArray<uint8> RawWaveData;
			SerializeWaveFile(RawWaveData, (uint8*)RawDataFromOGG.GetData(), RawDataFromOGG.Num()*2, NumChannels, SampleRate);

			FSharedBuffer Buffer = FSharedBuffer::Clone(RawWaveData.GetData(), RawWaveData.Num());

			SoundWave->RawData.UpdatePayload(Buffer);

			SoundWave->RawPCMDataSize = RawDataFromOGG.Num();
			SoundWave->RawPCMData = (uint8*)FMemory::Malloc(SoundWave->RawPCMDataSize);
			FMemory::Memcpy(SoundWave->RawPCMData, RawDataFromOGG.GetData(), SoundWave->RawPCMDataSize);

			// Calculate the duration of the sound wave
			// Note: We use the NumInputChannels for duration calculation since NumChannelsToSerialize may be 1 channel while NumInputChannels is 2 for the "split stereo" feature.
			SoundWave->Duration = Duration;
			SoundWave->SetSampleRate(SampleRate);
			SoundWave->NumChannels = NumChannels;
			SoundWave->SetSoundAssetCompressionType(ESoundAssetCompressionType::BinkAudio);
		}
		SoundWave->PostEditChange();
	}
	return SoundWave;
}

void ULegacySoundNodeWave::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	RawData.Serialize(Ar);
	FRedUELegacyByteBulkData* Bulks[] = {
		&CompressedPCData,
		&CompressedXbox360Data,
		&CompressedPS3Data,
		&CompressedWiiUData,		// appeared in ArVer 845
		&CompressedIPhoneData,		// appeared in ArVer 851
	};
	
	for (auto& Bulk : Bulks)
	{
		Bulk->Serialize(Ar);
		if (Ar.GetStopper() == Ar.Tell()) break;
	}
}

USoundNode* ULegacySoundNodeAttenuation::Export(USoundCue* SoundCue)
{
	if (USoundNodeAttenuation* Attenuation = SoundCue->ConstructSoundNode<USoundNodeAttenuation>())
	{
		if (ensure(ChildNodes.Num() == 1) && ChildNodes[0])
		{
			InsertNode(SoundCue, Attenuation, ChildNodes[0]->Export(SoundCue));
		}
		return Attenuation;
	}
	return nullptr;
}

USoundNode* ULegacySoundNodeConcatenator::Export(USoundCue* SoundCue)
{
	if (USoundNodeConcatenator* Concatenator = SoundCue->ConstructSoundNode<USoundNodeConcatenator>())
	{
		TArray<USoundNode*> Nodes;
		for (ULegacySoundNode* ChildNode : ChildNodes)
		{
			Nodes.Add(ChildNode->Export(SoundCue));
		}
		InsertNodes(SoundCue, Concatenator, Nodes);
		for (int32  i = 0, a = 0 ; i < ChildNodes.Num() && a < Concatenator->InputVolume.Num() ; i++)
		{
			if (Nodes[i])
			{
				Concatenator->InputVolume[a] = InputVolume[i];
				a++;
			}
		}
		return Concatenator;
	}
	return nullptr;
}

USoundNode* ULegacySoundNodeLooping::Export(USoundCue* SoundCue)
{
	if (USoundNodeLooping* Looping = SoundCue->ConstructSoundNode<USoundNodeLooping>())
	{
		if (ensure(ChildNodes.Num() == 1) && ChildNodes[0])
		{
			InsertNode(SoundCue, Looping, ChildNodes[0]->Export(SoundCue));
		}
		return Looping;
	}
	return nullptr;
}

UObject* ULegacySoundCue::ExportToContent()
{
	USoundCue* SoundCue;
	if(TryLoadOrCreate(SoundCue))
	{
		FAssetRegistryModule::AssetCreated(SoundCue);
		SoundCue->Modify();
		SoundCue->PreEditChange(nullptr);
		
		if (FirstNode)
		{
			SoundCue->FirstNode = FirstNode->Export(SoundCue);
			if (SoundCue->FirstNode )
			{
				SoundCue->LinkGraphNodesFromSoundNodes();
			}
		}
	
		
		SoundCue->PostEditChange();
	}
	return SoundCue;
}
