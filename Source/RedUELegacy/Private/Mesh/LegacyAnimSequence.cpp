#include "Mesh/LegacyAnimSequence.h"

#include "PackageTools.h"
#include "Animation/AnimSequence.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacyArchive.h"
#include "Mesh/LegacyAnimSet.h"
#include "Types/AttributeStorage.h"
#include "AssetRegistry/AssetRegistryModule.h"

void FLegacyRawAnimSequenceTrack::GetBonePosition(float Frame, float NumFrames, FVector3f& OutPosition, FQuat4f& OutQuat) const
{
	auto FindTimeKey = [](const TArray<float>& KeyTime, float Frame)-> int32
	{
		constexpr int32 MAX_LINEAR_KEYS = 4;
		// find index in time key array
		int32 NumKeys = KeyTime.Num();
		// *** binary search ***
		int32 Low = 0, High = NumKeys - 1;
		while (Low + MAX_LINEAR_KEYS < High)
		{
			int Mid = (Low + High) / 2;
			if (Frame < KeyTime[Mid]) High = Mid - 1;
			else Low = Mid;
		}
		// *** linear search ***
		int i;
		for (i = Low; i <= High; i++)
		{
			float CurrKeyTime = KeyTime[i];
			if (Frame == CurrKeyTime) return i; // exact key
			if (Frame < CurrKeyTime) return (i > 0) ? i - 1 : 0; // previous key
		}
		if (i > High) i = High;
		return i;
	};

	auto GetKeyParams = [FindTimeKey](const TArray<float>& KeyTime, float Frame, float NumFrames, int& X, int& Y, float& F)
	{
		X = FindTimeKey(KeyTime, Frame);
		Y = X + 1;
		int NumTimeKeys = KeyTime.Num();
		if (Y >= NumTimeKeys)
		{
			Y = NumTimeKeys - 1;
			check(X == Y);
			F = 0;
		}
		else
		{
			F = (Frame - KeyTime[X]) / (KeyTime[Y] - KeyTime[X]);
		}
	};

	if (KeyTimes.Num() == 1 || NumFrames == 1 || Frame == 0)
	{
		if (PosKeys.Num()) OutPosition = PosKeys[0];
		if (RotKeys.Num()) OutQuat = RotKeys[0];
		return;
	}

	// data for lerping
	int32 PositionX, RotationX; // index of previous frame
	int32 PositionY, RotationY; // index of next frame
	float PositionF, RotationF; // fraction between X and Y for lerping

	int32 NumTimeKeys = KeyTimes.Num();
	int32 NumPosKeys = PosKeys.Num();
	int32 NumRotKeys = RotKeys.Num();

	if (NumTimeKeys)
	{
		// here: KeyPos and KeyQuat sizes either equals to 1 or equals to KeyTime size
		check(NumPosKeys <= 1 || NumPosKeys == NumTimeKeys);
		check(NumRotKeys == 1 || NumRotKeys == NumTimeKeys);

		GetKeyParams(KeyTimes, Frame, NumFrames, PositionX, PositionY, PositionF);
		RotationX = PositionX;
		RotationY = PositionY;
		RotationF = PositionF;

		if (NumPosKeys <= 1)
		{
			PositionX = PositionY = 0;
			PositionF = 0;
		}
		if (NumRotKeys == 1)
		{
			RotationX = RotationY = 0;
			RotationF = 0;
		}
	}
	else
	{
		// empty KeyTime array - keys are evenly spaced on a time line
		// note: KeyPos and KeyQuat sizes can be different
		if (KeyPosTime.Num())
		{
			GetKeyParams(KeyPosTime, Frame, NumFrames, PositionX, PositionY, PositionF);
		}
		else if (NumPosKeys > 1)
		{
			float Position = Frame / NumFrames * NumPosKeys;
			PositionX = FMath::FloorToInt32(Position);
			PositionF = Position - PositionX;
			PositionY = PositionX + 1;
			if (PositionY >= NumPosKeys)
			{
				PositionY = NumPosKeys - 1;
				PositionF = 0;
			}
		}
		else
		{
			PositionX = PositionY = 0;
			PositionF = 0;
		}

		if (KeyQuatTime.Num())
		{
			GetKeyParams(KeyQuatTime, Frame, NumFrames, RotationX, RotationY, RotationF);
		}
		else if (NumRotKeys > 1)
		{
			float Position = Frame / NumFrames * NumRotKeys;
			RotationX = FMath::FloorToInt32(Position);
			RotationF = Position - RotationX;
			RotationY = RotationX + 1;
			if (RotationY >= NumRotKeys)
			{
				RotationY = NumRotKeys - 1;
				RotationF = 0;
			}
		}
		else
		{
			RotationX = RotationY = 0;
			RotationF = 0;
		}
	}

	// get position
	if (PositionF > 0)
	{
		OutPosition = FMath::Lerp(PosKeys[PositionX], PosKeys[PositionY], PositionF);
	}
	else if (NumPosKeys)
	{
		OutPosition = PosKeys[PositionX];
	}
	// get orientation
	if (RotationF > 0)
	{
		OutQuat = FQuat4f::Slerp(RotKeys[RotationX], RotKeys[RotationY], RotationF);
	}
	else if (NumRotKeys) // do not change DstQuat when no keys
	{
		OutQuat = RotKeys[RotationX];
	}
}

void ULegacyAnimSequence::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	if (Ar.LegacyVer >= 577)
	{
		Ar << RawAnimData; // this field was moved to RawAnimationData, RawAnimData is deprecated
	}
	Ar << CompressedByteStream;
}

struct FLegacyVectorIntervalFixed32
{
	unsigned X : 10, Y : 11, Z : 11;

	FVector3f ToVector(const FVector3f& Mins, const FVector3f& Ranges) const
	{
		FVector3f r;
		r.X = (X / 511.0f - 1.0f) * Ranges.X + Mins.X;
		r.Y = (Y / 1023.0f - 1.0f) * Ranges.Y + Mins.Y;
		r.Z = (Z / 1023.0f - 1.0f) * Ranges.Z + Mins.Z;
		return r;
	}

	friend FArchive& operator<<(FArchive& Ar, FLegacyVectorIntervalFixed32& V)
	{
		return Ar << *reinterpret_cast<uint32*>(&V);
	}
};

struct FLegacyQuatFloat96NoW
{
	float X, Y, Z;

	operator FQuat4f() const
	{
		FQuat4f r;
		r.X = X;
		r.Y = Y;
		r.Z = Z;

		float wSq = 1.0f - (r.X * r.X + r.Y * r.Y + r.Z * r.Z);
		r.W = (wSq > 0) ? sqrt(wSq) : 0;

		return r;
	}

	friend FArchive& operator<<(FArchive& Ar, FLegacyQuatFloat96NoW& Q)
	{
		return Ar << Q.X << Q.Y << Q.Z;
	}
};

struct FLegacyQuatFixed48NoW
{
	uint16 X, Y, Z; // uint16, corresponds to (float+1)*32767

	operator FQuat4f() const
	{
		FQuat4f r;
		r.X = (X - 32767) / 32767.0f;
		r.Y = (Y - 32767) / 32767.0f;
		r.Z = (Z - 32767) / 32767.0f;

		float wSq = 1.0f - (r.X * r.X + r.Y * r.Y + r.Z * r.Z);
		r.W = (wSq > 0) ? sqrt(wSq) : 0;

		return r;
	}

	friend FArchive& operator<<(FArchive& Ar, FLegacyQuatFixed48NoW& Q)
	{
		return Ar << Q.X << Q.Y << Q.Z;
	}
};


struct FLegacyQuatFixed32NoW
{
	unsigned Z : 10, Y : 11, X : 11;

	operator FQuat4f() const
	{
		FQuat4f r;
		r.X = X / 1023.0f - 1.0f;
		r.Y = Y / 1023.0f - 1.0f;
		r.Z = Z / 511.0f - 1.0f;
		float wSq = 1.0f - (r.X * r.X + r.Y * r.Y + r.Z * r.Z);
		r.W = (wSq > 0) ? sqrt(wSq) : 0;
		return r;
	}

	friend FArchive& operator<<(FArchive& Ar, FLegacyQuatFixed32NoW& Q)
	{
		return Ar << *reinterpret_cast<uint32*>(&Q);
	}
};


struct FLegacyQuatIntervalFixed32NoW
{
	unsigned Z : 10, Y : 11, X : 11;

	FQuat4f ToQuat(const FVector4f& Mins, const FVector4f& Ranges) const
	{
		FQuat4f r;
		r.X = (X / 1023.0f - 1.0f) * Ranges.X + Mins.X;
		r.Y = (Y / 1023.0f - 1.0f) * Ranges.Y + Mins.Y;
		r.Z = (Z / 511.0f - 1.0f) * Ranges.Z + Mins.Z;
		float wSq = 1.0f - (r.X * r.X + r.Y * r.Y + r.Z * r.Z);
		r.W = (wSq > 0) ? sqrt(wSq) : 0;
		return r;
	}

	friend FArchive& operator<<(FArchive& Ar, FLegacyQuatIntervalFixed32NoW& Q)
	{
		return Ar << *reinterpret_cast<uint32*>(&Q);
	}
};

struct FLegacyQuatFloat32NoW
{
	uint32 data;

	operator FQuat4f() const
	{
		FQuat4f r;

		int _X = data >> 21; // 11 bits
		int _Y = (data >> 10) & 0x7FF; // 11 bits
		int _Z = data & 0x3FF; // 10 bits

		*(unsigned*)&r.X = ((((_X >> 7) & 7) + 123) << 23) | ((_X & 0x7F | 32 * (_X & 0xFFFFFC00)) << 16);
		*(unsigned*)&r.Y = ((((_Y >> 7) & 7) + 123) << 23) | ((_Y & 0x7F | 32 * (_Y & 0xFFFFFC00)) << 16);
		*(unsigned*)&r.Z = ((((_Z >> 6) & 7) + 123) << 23) | ((_Z & 0x3F | 32 * (_Z & 0xFFFFFE00)) << 17);

		float wSq = 1.0f - (r.X * r.X + r.Y * r.Y + r.Z * r.Z);
		r.W = (wSq > 0) ? sqrt(wSq) : 0;
		return r;
	}

	friend FArchive& operator<<(FArchive& Ar, FLegacyQuatFloat32NoW& Q)
	{
		return Ar << Q.data;
	}
};

struct FLegacyVectorFixed48
{
	uint16 X, Y, Z;

	operator FVector3f() const
	{
		FVector3f r;
		static const float scale = 128.0f / 32767.0f;
		r.X = (X - 32767) * scale;
		r.Y = (Y - 32767) * scale;
		r.Z = (Z - 32767) * scale;
		return r;
	}

	friend FArchive& operator<<(FArchive& Ar, FLegacyVectorFixed48& V)
	{
		return Ar << V.X << V.Y << V.Z;
	}
};

UAnimSequence* ULegacyAnimSequence::CreateSequence(ULegacyAnimSet* OwnerAnimSet, USkeleton* Skeleton)
{
	if (PresentObject)
	{
		return CastChecked<UAnimSequence>(PresentObject);
	}
	FString PathToSkeleton = Skeleton->GetPathName();
	PathToSkeleton = FPaths::GetPath(PathToSkeleton) / FPaths::GetCleanFilename(PathToSkeleton) + TEXT("_Anims") / SequenceName.ToString();


	const FString PackageName = UPackageTools::SanitizePackageName(PathToSkeleton);
	const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
	UAnimSequence* AnimSequence = LoadObject<UAnimSequence>(nullptr, *FullObjectPath, nullptr, LOAD_NoWarn);
	if (!AnimSequence)
	{
		FillRawAnimData(OwnerAnimSet);

		UPackage* AssetPackage = CreatePackage(*PackageName);
		AnimSequence = NewObject<UAnimSequence>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public | RF_Standalone);
		AnimSequence->PreEditChange(nullptr);
		FAssetRegistryModule::AssetCreated(AnimSequence);
		AnimSequence->SetSkeleton(Skeleton);
		AnimSequence->SetPreviewMesh(Skeleton->GetPreviewMesh());

		{
			IAnimationDataController& Controller = AnimSequence->GetController();

			// If we should transact, we'll already have a transaction from somewhere else. We should suppress this because
			// it will also create a transaction when importing into UE assets, and the level sequence assets can emit some warnings about it
			const bool bShouldTransact = false;
			Controller.OpenBracket(NSLOCTEXT("RedUELegacy","UEPackegeImporterAnimData_Bracket", "UEPackegeImporter Animation Data"), bShouldTransact);
			Controller.ResetModel(bShouldTransact);

			Controller.InitializeModel();
			// Bake the animation for each frame.
			// An alternative route would be to convert the time samples into TransformCurves, add them to UAnimSequence::RawCurveData,
			// and then call UAnimSequence::BakeTrackCurvesToRawAnimation. Doing it this way provides a few benefits though: The main one is that the way with which
			// UAnimSequence bakes can lead to artifacts on problematic joints (e.g. 90 degree rotation joints children of -1 scale joints, etc.) as it compounds the
			// transformation with the rest pose. Another benefit is that that doing it this way lets us offload the interpolation to USD, so that it can do it
			// however it likes, and we can just sample the joints at the target framerate

			for (int32 BoneIndex = 0; BoneIndex < RawAnimData.Num(); ++BoneIndex)
			{
				FName BoneName = OwnerAnimSet->TrackBoneNames[BoneIndex];
				TArray<FVector> PosKeys;
				TArray<FQuat> RotKeys;
				TArray<FVector> ScaleKeys;
				for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
				{
					FVector3f PosKey;
					FQuat4f RotKey;
					FVector3f ScaleKey = {1,1,1};
					RawAnimData[BoneIndex].GetBonePosition(FrameIndex,NumFrames,PosKey,RotKey);
					PosKeys.Add(FVector(PosKey.X, PosKey.Y, PosKey.Z));
					RotKeys.Add(FQuat(RotKey.X, RotKey.Y, RotKey.Z, -RotKey.W));
					ScaleKeys.Add(FVector(ScaleKey.X, ScaleKey.Y, ScaleKey.Z));
				}

				Controller.AddBoneTrack(BoneName, bShouldTransact);
				Controller.SetBoneTrackKeys(BoneName, PosKeys, RotKeys, ScaleKeys, bShouldTransact);
			}

			AnimSequence->Interpolation = EAnimInterpolationType::Linear;
			float NumRate = NumFrames/SequenceLength;
			AnimSequence->ImportFileFramerate = NumRate;
			AnimSequence->ImportResampleFramerate = NumRate;
			AnimSequence->RateScale = RateScale;

			Controller.SetPlayLength(SequenceLength, bShouldTransact);

			FFrameRate FrameRate;
			FrameRate.Denominator = 1;
			for (; !FMath::IsNearlyZero(FMath::Frac(NumRate * FrameRate.Denominator)); FrameRate.Denominator *= 10)
			{
			}
			FrameRate.Numerator = FMath::FloorToInt32(NumRate * FrameRate.Denominator);

			Controller.SetFrameRate(FrameRate, bShouldTransact);
			Controller.NotifyPopulated(); // This call is important to get the controller to not use the sampling frequency as framerate
			Controller.CloseBracket(bShouldTransact);
		}
		AnimSequence->PostEditChange();
		AnimSequence->Modify();
	}
	PresentObject = AnimSequence;
	return AnimSequence;
}

void ULegacyAnimSequence::FillRawAnimData(ULegacyAnimSet* OwnerAnimSet)
{
	auto DecodeFixed48_PerTrackComponent = [](uint16 Value, int32 Log2)-> float
	{
		const int Offset = (1 << (15 - Log2)) - 1; // default (for Log2==7) is 255
		const float InvFactor = 1.0f / (Offset >> Log2); // default is 1.0f
		return (Value - Offset) * InvFactor;
	};

	auto ReadTimeArray = [](FArchive& Ar, int NumKeys, TArray<float>& Times, int NumFrames)
	{
		Times.Empty(NumKeys);
		if (NumKeys <= 1) return;

		//	appPrintf("  pos=%4X keys (max=%X)[ ", Ar.Tell(), NumFrames);
		if (NumFrames < 256)
		{
			for (int k = 0; k < NumKeys; k++)
			{
				uint8 v;
				Ar << v;
				Times.Add(v);
				//			if (k < 4 || k > NumKeys - 5) appPrintf(" %02X ", v);
				//			else if (k == 4) appPrintf("...");
			}
		}
		else
		{
			for (int k = 0; k < NumKeys; k++)
			{
				uint16 v;
				Ar << v;
				Times.Add(v);
				//			if (k < 4 || k > NumKeys - 5) appPrintf(" %04X ", v);
				//			else if (k == 4) appPrintf("...");
			}
		}
		//	appPrintf(" ]\n");

		// align to 4 bytes
		Ar.Seek(Align(Ar.Tell(), 4));
	};

	int32 NumTracks = OwnerAnimSet->TrackBoneNames.Num();
	if (!CompressedTrackOffsets.Num())
	{
		check(RawAnimData.Num() == NumTracks);
		return;
	}
	RawAnimData.Empty(NumTracks);
	RawAnimData.AddDefaulted(NumTracks);
	FMemoryReader Reader(CompressedByteStream);
	bool HasTimeTracks = (KeyEncodingFormat == ELegacyAnimationKeyFormat::AKF_VariableKeyLerp);

	int offsetsPerBone = 4;
	if (KeyEncodingFormat == ELegacyAnimationKeyFormat::AKF_PerTrackCompression) offsetsPerBone = 2;

	int32 offsetIndex = 0;
	for (int32 j = 0; j < NumTracks; j++, offsetIndex += offsetsPerBone)
	{
		FVector3f Mins, Ranges; // common ...

		FLegacyRawAnimSequenceTrack* A = &RawAnimData[j];
		//----------------------------------------------
		// decode AKF_PerTrackCompression data
		//----------------------------------------------
		if (KeyEncodingFormat == ELegacyAnimationKeyFormat::AKF_PerTrackCompression)
		{
			check(TranslationCompressionFormat == ACF_Identity);
			check(RotationCompressionFormat == ACF_Identity);

			int TransOffset = CompressedTrackOffsets[offsetIndex];
			int RotOffset = CompressedTrackOffsets[offsetIndex + 1];

			uint32 PackedInfo;
			AnimationCompressionFormat KeyFormat;
			int ComponentMask;
			int NumKeys;

#define DECODE_PER_TRACK_INFO(info)										\
				KeyFormat = (AnimationCompressionFormat)(info >> 28);	\
				ComponentMask = (info >> 24) & 0xF;						\
				NumKeys       = info & 0xFFFFFF;						\
				HasTimeTracks = (ComponentMask & 8) != 0;

			// read translation keys
			if (TransOffset == -1)
			{
				A->PosKeys.Add(FVector3f::ZeroVector);
			}
			else
			{
				Reader.Seek(TransOffset);
				Reader << PackedInfo;
				DECODE_PER_TRACK_INFO(PackedInfo);
				A->PosKeys.Empty(NumKeys);
				if (KeyFormat == ACF_IntervalFixed32NoW)
				{
					// read mins/maxs
					Mins.Set(0, 0, 0);
					Ranges.Set(0, 0, 0);
					if (ComponentMask & 1) Reader << Mins.X << Ranges.X;
					if (ComponentMask & 2) Reader << Mins.Y << Ranges.Y;
					if (ComponentMask & 4) Reader << Mins.Z << Ranges.Z;
				}
				for (int32 k = 0; k < NumKeys; k++)
				{
					switch (KeyFormat)
					{
					//						case ACF_None:
					case ACF_Float96NoW:
						{
							FVector3f v;
							if (ComponentMask & 7)
							{
								v.Set(0, 0, 0);
								if (ComponentMask & 1) Reader << v.X;
								if (ComponentMask & 2) Reader << v.Y;
								if (ComponentMask & 4) Reader << v.Z;
							}
							else
							{
								// ACF_Float96NoW has a special case for ((ComponentMask & 7) == 0)
								Reader << v;
							}
							A->PosKeys.Add(v);
						}
						break;
					case ACF_IntervalFixed32NoW:
						{
							FLegacyVectorIntervalFixed32 v;
							Reader << v;
							A->PosKeys.Add(v.ToVector(Mins, Ranges));
						}
						break;

					case ACF_Fixed48NoW:
						{
							uint16 X, Y, Z;
							FVector3f v;
							v.Set(0, 0, 0);
							if (ComponentMask & 1)
							{
								Reader << X;
								v[0] = DecodeFixed48_PerTrackComponent(X, 7);
							}
							if (ComponentMask & 2)
							{
								Reader << Y;
								v[1] = DecodeFixed48_PerTrackComponent(Y, 7);
							}
							if (ComponentMask & 4)
							{
								Reader << Z;
								v[2] = DecodeFixed48_PerTrackComponent(Z, 7);
							}
							A->PosKeys.Add(v);
						}
						break;
					case ACF_Identity:
						A->PosKeys.Add(FVector3f::ZeroVector);
						break;
					default:
						{
							checkNoEntry();
						}
					}
				}
				// align to 4 bytes
				Reader.Seek(Align(Reader.Tell(), 4));
				if (HasTimeTracks)
				{
					ReadTimeArray(Reader, NumKeys, A->KeyTimes, NumFrames);
				}
			}

			if (RotOffset == -1)
			{
				A->RotKeys.Add(FQuat4f::Identity);
			}
			else
			{
				Reader.Seek(RotOffset);
				Reader << PackedInfo;
				DECODE_PER_TRACK_INFO(PackedInfo);
				A->RotKeys.Empty(NumKeys);
				if (KeyFormat == ACF_IntervalFixed32NoW)
				{
					// read mins/maxs
					Mins.Set(0, 0, 0);
					Ranges.Set(0, 0, 0);
					if (ComponentMask & 1) Reader << Mins.X << Ranges.X;
					if (ComponentMask & 2) Reader << Mins.Y << Ranges.Y;
					if (ComponentMask & 4) Reader << Mins.Z << Ranges.Z;
				}
				for (int32 k = 0; k < NumKeys; k++)
				{
					switch (KeyFormat)
					{
					//						TR (ACF_None, FQuat)
					case ACF_Float96NoW:
						{
							FLegacyQuatFloat96NoW q;
							Reader << q;
							A->RotKeys.Add(q);
						}
						break;
					case ACF_Fixed48NoW:
						{
							FLegacyQuatFixed48NoW q;
							q.X = q.Y = q.Z = 32767; // corresponds to 0
							if (ComponentMask & 1) Reader << q.X;
							if (ComponentMask & 2) Reader << q.Y;
							if (ComponentMask & 4) Reader << q.Z;
							A->RotKeys.Add(q);
						}
						break;
					case ACF_Fixed32NoW:
						{
							FLegacyQuatFixed32NoW q;
							Reader << q;
							A->RotKeys.Add(q);
						}
						break;
					case ACF_IntervalFixed32NoW:
						{
							FLegacyQuatIntervalFixed32NoW q;
							Reader << q;
							A->RotKeys.Add(q.ToQuat(Mins, Ranges));
						}
						break;
					case ACF_Float32NoW:
						{
							FLegacyQuatFloat32NoW q;
							Reader << q;
							A->RotKeys.Add(q);
						}
						break;

					case ACF_Identity:
						A->RotKeys.Add(FQuat4f::Identity);
						break;
					default: checkNoEntry()
					}
				}
				// align to 4 bytes
				Reader.Seek(Align(Reader.Tell(), 4));
				if (HasTimeTracks)
				{
					ReadTimeArray(Reader, NumKeys, A->KeyQuatTime, NumFrames);
				}
			}
			continue;
			// end of AKF_PerTrackCompression block ...
		}

		//----------------------------------------------
		// end of AKF_PerTrackCompression decoder
		//----------------------------------------------

		// read animations
		int TransOffset = CompressedTrackOffsets[offsetIndex];
		int TransKeys = CompressedTrackOffsets[offsetIndex + 1];
		int RotOffset = CompressedTrackOffsets[offsetIndex + 2];
		int RotKeys = CompressedTrackOffsets[offsetIndex + 3];
		//			appPrintf("[%d:%d:%d] :  %d[%d]  %d[%d]  %d[%d]\n", j, Seq->RotationCompressionFormat, Seq->TranslationCompressionFormat, TransOffset, TransKeys, RotOffset, RotKeys, ScaleOffset, ScaleKeys);

		A->PosKeys.Empty(TransKeys);
		A->RotKeys.Empty(RotKeys);

		// read translation keys
		if (TransKeys)
		{
			
			AnimationCompressionFormat LocalTranslationCompressionFormat = TranslationCompressionFormat;
			Reader.Seek(TransOffset);
			if (TransKeys == 1)
			{
				LocalTranslationCompressionFormat = ACF_None; // single key is stored without compression
			}
			
			// read mins/ranges
			if (LocalTranslationCompressionFormat == ACF_IntervalFixed32NoW)
			{
				Reader << Mins << Ranges;
			}
			for (int32 k = 0; k < TransKeys; k++)
			{
				switch (LocalTranslationCompressionFormat)
				{
				case ACF_None:
					{
						FVector3f v;
						Reader << v;
						A->PosKeys.Add(v);
					}
					break;
				case ACF_Float96NoW:
					{
						FVector3f v;
						Reader << v;
						A->PosKeys.Add(v);
					}
					break;
				case ACF_IntervalFixed32NoW:
					{
						FLegacyVectorIntervalFixed32 v;
						Reader << v;
						A->PosKeys.Add(v.ToVector(Mins, Ranges));
					}
					break;
				case ACF_Fixed48NoW:
					{
						FLegacyVectorFixed48 v;
						Reader << v;
						A->PosKeys.Add(v);
					}
					break;

				case ACF_Identity:
					A->PosKeys.Add(FVector3f::ZeroVector);
					break;
				default: checkNoEntry();
				}
			}

			// align to 4 bytes
			Reader.Seek(Align(Reader.Tell(), 4));
			if (HasTimeTracks) ReadTimeArray(Reader, TransKeys, A->KeyPosTime, NumFrames);
		}
		// read rotation keys
		Reader.Seek(RotOffset);
		if (RotKeys > 0)
		{
			AnimationCompressionFormat LocalRotationCompressionFormat = RotationCompressionFormat;
			if (RotKeys == 1)
			{
				LocalRotationCompressionFormat = ACF_Float96NoW; // single key is stored without compression
			}
			else if (LocalRotationCompressionFormat == ACF_IntervalFixed32NoW || LegacyPackage->LegacyVer < 761)
			{
				// starting with version 761 Mins/Ranges are read only when needed - i.e. for ACF_IntervalFixed32NoW
				Reader << Mins << Ranges;
			}


			for (int32 k = 0; k < RotKeys; k++)
			{
				switch (LocalRotationCompressionFormat)
				{
				case ACF_None:
					{
						FQuat4f q;
						Reader << q;
						A->RotKeys.Add(q);
					}
					break;
				case ACF_Float96NoW:
					{
						FLegacyQuatFloat96NoW q;
						Reader << q;
						A->RotKeys.Add(q);
					}
					break;
				case ACF_Fixed48NoW:
					{
						FLegacyQuatFixed48NoW q;
						Reader << q;
						A->RotKeys.Add(q);
					}
					break;
				case ACF_Fixed32NoW:
					{
						FLegacyQuatFixed32NoW q;
						Reader << q;
						A->RotKeys.Add(q);
					}
					break;
				case ACF_IntervalFixed32NoW:
					{
						FLegacyQuatIntervalFixed32NoW q;
						Reader << q;
						A->RotKeys.Add(q.ToQuat(Mins, Ranges));
					}
					break;
				case ACF_Float32NoW:
					{
						FLegacyQuatFloat32NoW q;
						Reader << q;
						A->RotKeys.Add(q);
					}
					break;
				case ACF_Identity:
					A->RotKeys.Add(FQuat4f::Identity);
					break;

				default: checkNoEntry();
				}
			}
		}
		// align to 4 bytes
		Reader.Seek(Align(Reader.Tell(), 4));
		if (HasTimeTracks) ReadTimeArray(Reader, RotKeys, A->KeyQuatTime, NumFrames);
	}
}
