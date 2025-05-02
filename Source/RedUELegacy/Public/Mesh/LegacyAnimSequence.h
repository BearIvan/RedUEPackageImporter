#pragma once

#include "Core/LegacyObject.h"
#include "Core/RedUELegacyArchive.h"
#include "LegacyAnimSequence.generated.h"

class ULegacyAnimSet;

UENUM()
enum class ELegacyAnimationKeyFormat
{
	AKF_ConstantKeyLerp,									// animation keys are placed on evenly-spaced intervals
	AKF_VariableKeyLerp,									// animation keys have explicit key times
	AKF_PerTrackCompression,
};

struct FLegacyRawAnimSequenceTrack
{
	TArray<FVector3f>		PosKeys;
	TArray<FQuat4f>			RotKeys;
	TArray<float>			KeyTimes;
	TArray<float> 			KeyPosTime;
	TArray<float> 			KeyQuatTime;
	void GetBonePosition(float Frame, float NumFrames, FVector3f &OutPosition, FQuat4f &OutQuat) const;
	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyRawAnimSequenceTrack &T)
	{
		if (Ar.LegacyVer >= 577)
        {
			Ar.LegacyBulkSerialize(T.PosKeys);
			Ar.LegacyBulkSerialize(T.RotKeys);
        	if (Ar.LegacyVer < 604)  Ar.LegacyBulkSerialize(T.KeyTimes);
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
	
	virtual void 						LegacySerialize	(FRedUELegacyArchive& Ar) override;
			UAnimSequence*				CreateSequence	(ULegacyAnimSet* OwnerAnimSet,USkeleton* Skeleton);
			 		
	TArray<uint8> 						CompressedByteStream;
	TArray<FLegacyRawAnimSequenceTrack> RawAnimData;
	
private:
	void FillRawAnimData(ULegacyAnimSet* OwnerAnimSet);
public:
	UPROPERTY(BlueprintReadWrite)
	FName SequenceName;
	
	UPROPERTY(BlueprintReadWrite)
	float SequenceLength;
	
	UPROPERTY(BlueprintReadWrite)
	int NumFrames;
	
	UPROPERTY(BlueprintReadWrite)
	float RateScale;
	
	UPROPERTY(BlueprintReadWrite)
	bool bNoLoopingInterpolation;
	
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<AnimationCompressionFormat> TranslationCompressionFormat;
	
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<AnimationCompressionFormat> RotationCompressionFormat;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyAnimationKeyFormat KeyEncodingFormat;				// GoW2+
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsAdditive;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<int32> CompressedTrackOffsets;
	
	UPROPERTY(BlueprintReadWrite)
	FName AdditiveRefName;
};
