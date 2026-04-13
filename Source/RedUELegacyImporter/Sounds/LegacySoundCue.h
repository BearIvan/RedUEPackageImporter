#pragma once

#include "Core/LegacyObject.h"
#include "Core/LegacyPackage.h"
#include "LegacySoundCue.generated.h"

/**
 * 
 */


UCLASS()
class REDUELEGACYIMPORTER_API ULegacySoundNode : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual USoundNode* Export(USoundCue* SoundCue);
	void InsertNodes(USoundCue* SoundCue,USoundNode* To,const TArray<USoundNode*>& Nodes);
	void InsertNode(USoundCue* SoundCue,USoundNode* To,USoundNode* Node);
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySoundNodeWave : public ULegacySoundNode
{
	GENERATED_BODY()
public:
	virtual USoundNode* Export(USoundCue* SoundCue) override;
	virtual UObject* ExportToContent() override;
	virtual void LegacySerialize(FRedUELegacyArchive& Ar) override;

	UPROPERTY(meta = (LegacyRead))
	bool bLoopingSound;
	
	FRedUELegacyByteBulkData		RawData;
	FRedUELegacyByteBulkData		CompressedPCData;
	FRedUELegacyByteBulkData		CompressedXbox360Data;
	FRedUELegacyByteBulkData		CompressedPS3Data;
	FRedUELegacyByteBulkData		CompressedWiiUData;
	FRedUELegacyByteBulkData		CompressedIPhoneData;
	
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySoundNodeAttenuation : public ULegacySoundNode
{
	GENERATED_BODY()
public:
	virtual USoundNode* Export(USoundCue* SoundCue) override;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<ULegacySoundNode*> ChildNodes;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySoundNodeConcatenator: public ULegacySoundNode
{
	GENERATED_BODY()
public:
	virtual USoundNode* Export(USoundCue* SoundCue) override;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<float> InputVolume;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<ULegacySoundNode*> ChildNodes;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySoundNodeLooping: public ULegacySoundNode
{
	GENERATED_BODY()
public:
	virtual USoundNode* Export(USoundCue* SoundCue) override;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<ULegacySoundNode*> ChildNodes;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySoundCue : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual UObject* ExportToContent  () override;
	
	UPROPERTY(meta = (LegacyRead))
	ULegacySoundNode* FirstNode;
	
	UPROPERTY(meta = (LegacyRead))
	float  VolumeMultiplier;
};
