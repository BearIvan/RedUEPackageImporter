#pragma once

#include "LegacyStaticMesh3.h"
#include "Core/LegacyObject.h"
#include "Core/RedUELegacyArchive.h"
#include "LegacySkeletalMesh3.generated.h"



struct FLegacySkeletalMeshSection3
{
	int16				MaterialIndex;
	int16				ChunkIndex;
	int32				FirstIndex;
	int32				NumTriangles;
	uint8				unk2;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacySkeletalMeshSection3 &S)
	{
		if (Ar.LegacyVer < 215)
		{
			// UE2 fields
			int16 FirstIndex;
			int16 unk1, unk2, unk3, unk4, unk5, unk6;
			TArray<int16> unk8;
			Ar << S.MaterialIndex << FirstIndex << unk1 << unk2 << unk3 << unk4 << unk5 << unk6 << S.NumTriangles;
			if (Ar.LegacyVer < 202) Ar << unk8;
			S.FirstIndex = FirstIndex;
			S.ChunkIndex = 0;
			return Ar;
		}
		Ar << S.MaterialIndex << S.ChunkIndex << S.FirstIndex;
		if (Ar.LegacyVer < 806)
		{
			uint16 NumTriangles;
			Ar << NumTriangles;
			S.NumTriangles = NumTriangles;
		}
		else
		{
			Ar << S.NumTriangles;
		}
		
		if (Ar.LegacyVer >= 599)
		{
			Ar << S.unk2;
		}
		return Ar;
	}
};
struct FLegacyRigidVertex3
{
	FVector				Pos;
	FPackedNormal		Normal[3];
	FLegacyMeshUVFloat	UV[NUM_UV_SETS_UE3];
	uint8				BoneIndex;
	int32				Color;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyRigidVertex3 &V)
	{
		int NumUVSets = 1;

		Ar << V.Pos;

		Ar << V.Normal[0] << V.Normal[1] << V.Normal[2];

		if (Ar.LegacyVer >= 709) NumUVSets = 4;

		for (int i = 0; i < NumUVSets; i++)
			Ar << V.UV[i];

		if (Ar.LegacyVer >= 710) Ar << V.Color;	

		Ar << V.BoneIndex;
		return Ar;
	}
};


struct FLegacySoftVertex3
{
	FVector				Pos;
	FPackedNormal		Normal[3];
	FLegacyMeshUVFloat	UV[NUM_UV_SETS_UE3];
	uint8				BoneIndex[NUM_INFLUENCES_UE3];
	uint8				BoneWeight[NUM_INFLUENCES_UE3];
	int32					Color;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacySoftVertex3 &V)
	{
		int32 NumUVSets = 1;
		Ar << V.Pos;
		Ar << V.Normal[0] << V.Normal[1] << V.Normal[2];
		if (Ar.LegacyVer >= 709) NumUVSets = 4;
		for (int i = 0; i < NumUVSets; i++)
			Ar << V.UV[i];

		if (Ar.LegacyVer >= 710) Ar << V.Color;	
		if (Ar.LegacyVer >= 333)
		{
			for (int32 i = 0; i < NUM_INFLUENCES_UE3; i++) Ar << V.BoneIndex[i];
			for (int32 i = 0; i < NUM_INFLUENCES_UE3; i++) Ar << V.BoneWeight[i];
		}
		else
		{
			for (int32 i = 0; i < NUM_INFLUENCES_UE3; i++)
				Ar << V.BoneIndex[i] << V.BoneWeight[i];
		}
		return Ar;
	}
};

struct FLegacySkeletalMeshChunk3
{
	int32						FirstVertex;
	TArray<FLegacyRigidVertex3>	RigidVerts;
	TArray<FLegacySoftVertex3>	SoftVerts;
	TArray<int16>				Bones;
	int32						NumRigidVerts;
	int32						NumSoftVerts;
	int32						MaxInfluences;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar,FLegacySkeletalMeshChunk3 &V)
	{
		Ar << V.FirstVertex ;
		Ar << V.RigidVerts;
		Ar << V.SoftVerts;
		Ar << V.Bones;
		if (Ar.LegacyVer >= 333)
		{
			Ar << V.NumRigidVerts << V.NumSoftVerts;
		}
		else
		{
			V.NumRigidVerts = V.RigidVerts.Num();
			V.NumSoftVerts = V.SoftVerts.Num();
		}
		if (Ar.LegacyVer >= 362)
		{
			Ar << V.MaxInfluences;
		}
		return Ar;
	}
};


struct FLegacySkeletalMeshIndexBuffer3
{
	TArray<uint16>		Indices16;
	TArray<uint32>		Indices32;

	FORCEINLINE bool Is32Bit() const
	{
		return (Indices32.Num() != 0);
	}

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacySkeletalMeshIndexBuffer3 &I)
	{
		uint8 ItemSize = 2;

		if (Ar.LegacyVer >= 806)
		{
			int32		f0;
			Ar << f0 << ItemSize;
		}
		if (ItemSize == 2)
		{
			Ar<<I.Indices16;
		}
		else
		{
			check (ItemSize == 4);
			Ar<<I.Indices32;
		}

		int32 unk;
		if (Ar.LegacyVer < 297) Ar << unk;

		return Ar;
	}
};

struct FLegacySkeletalMeshLODModel3
{
	TArray<FLegacySkeletalMeshSection3> Sections;
	TArray<FLegacySkeletalMeshChunk3>	Chunks;
	FLegacySkeletalMeshIndexBuffer3		IndexBuffer;
};



UCLASS()
class REDUELEGACY_API ULegacySkeletalMesh3 : public ULegacyObject
{
	GENERATED_BODY()
public:
										ULegacySkeletalMesh3				();
	virtual                             ~ULegacySkeletalMesh3               () override;
	virtual FName                       GetLegacyClassName_Implementation   (ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual bool                        LegacySupport_Implementation        (ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual void                        LegacySerialize                     (FRedUELegacyArchive& Ar) override;
	virtual UObject*                    ExportToContent                     () override;
};
