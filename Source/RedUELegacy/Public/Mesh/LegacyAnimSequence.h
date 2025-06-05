#pragma once

#include "Core/LegacyObject.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacyArchive.h"
#include "LegacyAnimSequence.generated.h"

class ULegacyAnimSet;

UENUM()
enum class ELegacyAnimationKeyFormat
{
	AKF_ConstantKeyLerp, // animation keys are placed on evenly-spaced intervals
	AKF_VariableKeyLerp, // animation keys have explicit key times
	AKF_PerTrackCompression,
};

struct FLegacyRawAnimSequenceTrack
{
	TArray<FVector3f>	PosKeys;
	TArray<FQuat4f>		RotKeys;
	TArray<float>		KeyTimes;
	TArray<float>		KeyPosTime;
	TArray<float>		KeyQuatTime;
	void GetBonePosition(float Frame, float NumFrames, FVector3f& OutPosition, FQuat4f& OutQuat) const;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyRawAnimSequenceTrack& T)
	{
		if (Ar.LegacyVer >= 577)
		{
			Ar.LegacyBulkSerialize(T.PosKeys);
			Ar.LegacyBulkSerialize(T.RotKeys);
			if (Ar.LegacyVer < 604) Ar.LegacyBulkSerialize(T.KeyTimes);
			return Ar;
		}
		return Ar << T.PosKeys << T.RotKeys << T.KeyTimes;
	}
};

UCLASS()
class REDUELEGACY_API ULegacyAnimSequence : public ULegacyObject
{
	GENERATED_BODY()

public:
	virtual void								LegacySerialize	(FRedUELegacyArchive& Ar) override;
			UAnimSequence*						CreateSequence	(ULegacyAnimSet* OwnerAnimSet, USkeleton* Skeleton);
					
			TArray<uint8>						CompressedByteStream;
			TArray<FLegacyRawAnimSequenceTrack> RawAnimData;

protected:
	virtual void								BuildAnimation	(USkeleton* Skeleton, ULegacyAnimSet* OwnerAnimSet, IAnimationDataController& Controller, bool bShouldTransact);
	virtual void								FillRawAnimData	(ULegacyAnimSet* OwnerAnimSet);
	virtual bool								CanImport		();

public:
	UPROPERTY(BlueprintReadWrite)
	FName SequenceName;

	UPROPERTY(BlueprintReadWrite)
	float SequenceLength;

	UPROPERTY(BlueprintReadWrite)
	int NumFrames;

	UPROPERTY(BlueprintReadWrite)
	float RateScale = 1;

	UPROPERTY(BlueprintReadWrite)
	bool bNoLoopingInterpolation;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<AnimationCompressionFormat> TranslationCompressionFormat;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<AnimationCompressionFormat> RotationCompressionFormat;

	UPROPERTY(BlueprintReadWrite)
	ELegacyAnimationKeyFormat KeyEncodingFormat; // GoW2+

	UPROPERTY(BlueprintReadWrite)
	bool bIsAdditive;

	UPROPERTY(BlueprintReadWrite)
	TArray<int32> CompressedTrackOffsets;

	UPROPERTY(BlueprintReadWrite)
	FName AdditiveRefName;
};

struct FLegactMorphemeAnimSourceBase
{
	int32 AnimationType;
	int64 FuncTable;
	int32 AnimationSize;
	int32 TrajectorySize;
	int32 ChannelNamesSize;
	int32 SectionsSize;
};

static_assert(sizeof(FLegactMorphemeAnimSourceBase) == 0x20);

struct FLegacyMorphemeQuantisationInfoQSA
{
	FVector3f Min;
	FVector3f Max;
};

struct FLegacyMorphemeUnchangingKeyQSA
{
	uint16 X;
	uint16 Y;
	uint16 Z;
};

struct FLegacyMorphemeAnimChannelSetInfoQSA
{
	FQuat4f								RootRotation;
	uint16 								WorldSpaceRootID;
	uint16 								CountChannelSets;
	uint16 								UnchangingPosNumChannels;
	uint16 								UnchangingQuatNumChannels;
	FLegacyMorphemeQuantisationInfoQSA	UnchangingPosQuantisationInfo;
	FLegacyMorphemeQuantisationInfoQSA	UnchangingQuatQuantisationInfo;
	FLegacyMorphemeUnchangingKeyQSA*	UnchangingPosData;
	FLegacyMorphemeUnchangingKeyQSA*	UnchangingQuatData;
	uint16								SampledPositionNumChannels;
	uint16								SampledQuatNumChannels;
	uint16								SplinePositionNumChannels;
	uint16								SplineQuatNumChannels;
	uint16*								ChannelPositionInfo;
	uint16*								ChannelQuatInfo;
	void Unpack();
};

static_assert(sizeof(FLegacyMorphemeAnimChannelSetInfoQSA) == 0x70);

struct FLegacyMorphemeQuantisationDataQSA
{
	uint8_t Prec[3];
	uint8_t Mean[3];
	uint8_t QuantisationSetID[3];
};

struct FLegacyMorphemeAnimSectionQSA
{
	uint16								SectionSize;
	uint16								NumChannelSets;
	uint16								SectionAnimStartFrame;
	uint16								SectionNumAnimFrames;
	uint16								SampledPosNumChannels;
	uint16								SampledQuatNumChannels;
	uint16								SplineNumKnots;
	uint16								SplinePosNumChannels;
	uint16								SplineQuatNumChannels;
	uint16								SampledPosByteStride;
	uint16								SampledQuatByteStride;
	uint16								SplinePosByteStride;
	uint16								SplineQuatWNegsByteStride;
	uint16								SplineQuatKeysByteStride;
	uint16								SplineQuatTangentsByteStride;
	uint16								SampledPosNumQuantisationSets;
	uint16								SampledQuatNumQuantisationSets;
	uint16								SplinePosNumQuantisationSets;
	uint16								SplineQuatKeysNumQuantisationSets;
	uint16								SplineQuatTangentsNumQuantisationSets;
	FLegacyMorphemeQuantisationInfoQSA	m_posMeansQuantisationInfo;
	FLegacyMorphemeQuantisationInfoQSA	SplineQuatTangentMeansQuantisationInfo;
	FLegacyMorphemeQuantisationInfoQSA* SampledPosQuantisationInfo;
	FLegacyMorphemeQuantisationDataQSA* SampledPosQuantisationData;
	uint8*								SampledPosData;
	FLegacyMorphemeQuantisationInfoQSA* SampledQuatQuantisationInfo;
	FLegacyMorphemeQuantisationDataQSA* SampledQuatQuantisationData;
	uint8*								SampledQuatData;
	uint16*								SplineKnots;
	FLegacyMorphemeQuantisationInfoQSA* SplinePosQuantisationInfo;
	FLegacyMorphemeQuantisationDataQSA* SplinePosQuantisationData;
	uint8*								SplinePosData;
	FLegacyMorphemeQuantisationInfoQSA* SplineQuatKeysQuantisationInfo;
	FLegacyMorphemeQuantisationDataQSA* SplineQuatKeysQuantisationData;
	uint8*								SplineQuatWNegsData;
	uint8*								SplineQuatKeysData;
	FLegacyMorphemeQuantisationInfoQSA* SplineQuatTangentsQuantisationInfo;
	FLegacyMorphemeQuantisationDataQSA* SplineQuatTangentsQuantisationData;
	uint8*								SplineQuatTangentsData;
	void								Unpack();
};

static_assert(sizeof(FLegacyMorphemeAnimSectionQSA) == 0xE0);

struct FLegacyMorphemeAnimSectionInfoQSA
{
	int32	StartFrame;
	int32	SectionSize;
	uint16	CacheIndex;
};

static_assert(sizeof(FLegacyMorphemeAnimSectionInfoQSA) == 0xC);


struct FLegacyMorphemeAnimSectionData
{
	TArray<uint8> Data;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyMorphemeAnimSectionData& S)
	{
		int32 Size = S.Data.Num();
		Ar << Size;
		S.Data.SetNum(Size);
		Ar.Serialize(S.Data.GetData(), Size);
		return Ar;
	}
};
struct FLegacyMorphemeTrajectorySourceBase
{
	int32 m_trajType;
	void* Unknown0;
	int32 Unknown1[4];
};
static_assert(sizeof(FLegacyMorphemeTrajectorySourceBase) == 0x20);



struct FLegacyMorphemeTrajectoryPosKeyNSA
{
	uint32 Data;
};

struct FLegacyMorphemeTrajectoryQuatKeyNSA
{
	uint16 X;
	uint16 Y;
	uint16 Z;
};


struct FLegacyMorphemeTrajectorySourceQSA: FLegacyMorphemeTrajectorySourceBase
{
	float m_sampleFrequency;
	int16 m_numAnimFrames;
	int16 m_flags;
	FLegacyMorphemeQuantisationInfoQSA m_sampledDeltaPosKeysInfo;
	FLegacyMorphemeQuantisationInfoQSA m_sampledDeltaQuatKeysInfo;
	FLegacyMorphemeTrajectoryPosKeyNSA * m_sampledDeltaPosKeys;
	FLegacyMorphemeTrajectoryQuatKeyNSA * m_sampledDeltaQuatKeys;
	void Unpack();
};
static_assert(sizeof(FLegacyMorphemeTrajectorySourceQSA) == 0x68);

struct FLegacyMorphemeAnimSourceTypeQSA : FLegactMorphemeAnimSourceBase
{
	float									Duration;
	float									SampleFrequency;
	int32									NumChannelSets;
	int32									NumSections;
	FLegacyMorphemeAnimSectionInfoQSA*		SectionsInfo;
	FLegacyMorphemeAnimChannelSetInfoQSA*	ChannelSetsInfo;
	FLegacyMorphemeAnimSectionQSA**			Sections;
	FLegacyMorphemeTrajectorySourceQSA*		TrajectoryData;
	void*									ChannelNames;
	void									Unpack(const TArray<FLegacyMorphemeAnimSectionData>& SectionData, int32 AnimDataSize);
};

static_assert(sizeof(FLegacyMorphemeAnimSourceTypeQSA) == 0x58);

struct FLegacyMorphemeQuantisationInfoNSA
{
	FVector3f Min;
	FVector3f Scale;
};

static_assert(sizeof(FLegacyMorphemeQuantisationInfoNSA) == 0x18);

struct FLegacyMorphemeUnchangingKeyNSA
{
	uint16 X;
	uint16 Y;
	uint16 Z;
};

static_assert(sizeof(FLegacyMorphemeUnchangingKeyNSA) == 0x6);

struct FLegacyMorphemeAnimChannelSetInfoNSA
{
	uint16								NumChannelSets;
	FLegacyMorphemeQuantisationInfoNSA	UnchangingPosQuantisationInfo;
	FLegacyMorphemeQuantisationInfoNSA	UnchangingQuatQuantisationInfo;
	FLegacyMorphemeUnchangingKeyNSA*	UnchangingPosData;
	FLegacyMorphemeUnchangingKeyNSA*	UnchangingQuatData;
	uint16								UnchangingPosNumChannels;
	uint16								UnchangingQuatNumChannels;
	uint16								SampledPosNumChannels;
	uint16								SampledQuatNumChannels;
	uint16*								ChannelPosInfo;
	uint16*								ChannelQuatInfo;
	void Unpack();
};

static_assert(sizeof(FLegacyMorphemeAnimChannelSetInfoNSA) == 0x60);

struct FLegacyMorphemeSampledPosKeyNSA
{
	uint32 Data;
};

struct FLegacyMorphemeSampledQuatKeyNSA
{
	uint16 X;
	uint16 Y;
	uint16 Z;
};

struct FLegacyMorphemeQuantisationDataNSA
{
	uint8_t Mean[3];
	uint8_t QuantisationSet[3];
};

struct FLegacyMorphemeAnimSubSectionNSA
{
	int64								SubsectionSize;
	int32								SubsectionID;
	uint16								SectionNumAnimFrames;
	uint16								SampledPosNumChannels;
	uint16								OffsetSampledPosNumChans;
	uint16								SampledQuatNumChannels;
	uint16								OffsetSampledQuatNumChans;
	uint16								SampledPosNumQuantisationSets;
	uint16								SampledQuatNumQuantisationSets;
	FLegacyMorphemeQuantisationInfoNSA	PosMeansQuantisationInfo;
	FLegacyMorphemeQuantisationInfoNSA* SampledPosQuantisationInfo;
	FLegacyMorphemeQuantisationDataNSA* SampledPosQuantisationData;
	FLegacyMorphemeSampledPosKeyNSA*	SampledPosData;
	FLegacyMorphemeQuantisationInfoNSA* SampledQuatQuantisationInfo;
	FLegacyMorphemeQuantisationDataNSA* SampledQuatQuantisationData;
	FLegacyMorphemeSampledQuatKeyNSA*	SampledQuatData;
	void								Unpack();
};

static_assert(sizeof(FLegacyMorphemeAnimSubSectionNSA) == 0x68);

struct FLegacyMorphemeAnimSectionNSA
{
	int32								NumChannelSets;
	int32								NumSubSections;
	int32								MaxSubsectionSize;
	FLegacyMorphemeAnimSubSectionNSA**	Subsections;
	void								Unpack();
};

static_assert(sizeof(FLegacyMorphemeAnimSectionNSA) == 0x18);

struct FLegactMorphemeAnimSectionInfoNSA
{
	int32 StartFrame;
	int32 SectionSize;
	int16 HeaderSize;
	int16 CacheIndex;
};

static_assert(sizeof(FLegactMorphemeAnimSectionInfoNSA) == 0xC);

struct FLegacyMorphemeAnimSourceTypeNSA : FLegactMorphemeAnimSourceBase
{
	float									Duration;
	float									SampleFrequency;
	int32									NumChannelSets;
	int32									NumSections;
	FLegactMorphemeAnimSectionInfoNSA*		SectionsInfo;
	FLegacyMorphemeAnimChannelSetInfoNSA*	ChannelSetsInfo;
	FLegacyMorphemeAnimSectionNSA**			Sections;
	//NSA too
	FLegacyMorphemeTrajectorySourceQSA*		TrajectoryData;
	void*									ChannelNames;
	void									Unpack(const TArray<FLegacyMorphemeAnimSectionData>& SectionData, int32 AnimDataSize);
};

static_assert(sizeof(FLegacyMorphemeAnimSourceTypeNSA) == 0x58);




UCLASS()
class REDUELEGACY_API ULegacyMorphemeAnimSequence : public ULegacyAnimSequence
{
	GENERATED_BODY()

public:
	virtual void									FillRawAnimData		(ULegacyAnimSet* OwnerAnimSet) override;
	virtual void									LegacySerialize		(FRedUELegacyArchive& Ar) override;
	virtual void									BuildAnimation		(USkeleton* Skeleton, ULegacyAnimSet* OwnerAnimSet, IAnimationDataController& Controller, bool bShouldTransact) override;
	virtual bool									CanImport			() override;
	
			FRedUELegacyByteBulkData				AnimSourceData;
			FRedUELegacyByteBulkData				CompiledAnimDataPC32;
			FRedUELegacyByteBulkData				CompiledAnimDataPC64;
			FRedUELegacyByteBulkData				CompiledAnimDataX360;
			FRedUELegacyByteBulkData				CompiledAnimDataPS3;
			TArray<FLegacyMorphemeAnimSectionData>	CompiledAnimSectionDataPC32;
			TArray<FLegacyMorphemeAnimSectionData>	CompiledAnimSectionDataPC64;
			TArray<FLegacyMorphemeAnimSectionData>	CompiledAnimSectionDataX360;
			TArray<FLegacyMorphemeAnimSectionData>	CompiledAnimSectionDataPS3;
	UPROPERTY(BlueprintReadWrite)
	int32 AnimationFormat = 2;
};
