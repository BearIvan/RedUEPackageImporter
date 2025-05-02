#pragma once

#include "LegacyStaticMesh3.h"
#include "Core/LegacyObject.h"
#include "Core/RedUELegacyArchive.h"
#include "LegacySkeletalMesh3.generated.h"


struct FLegacySkeletalMeshSection3
{
	int16 MaterialIndex;
	int16 ChunkIndex;
	int32 FirstIndex;
	int32 NumTriangles;
	uint8 unk2;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacySkeletalMeshSection3& S)
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
		if (Ar.Game == ERedUELegacyGame::Bioshock3)
		{ 
			//!! the same code as for MassEffect3, combine?
			uint8 SomeFlag;
			Ar << SomeFlag;
			ensure(SomeFlag == 0);
			// has extra data here in a case of SomeFlag != 0
		}
		return Ar;
	}
};

struct FLegacyRigidVertex3
{
	FVector3f Pos;
	FPackedNormal Normal[3];
	FLegacyMeshUVFloat UV[NUM_UV_SETS_UE3];
	uint8 BoneIndex;
	int32 Color;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyRigidVertex3& V)
	{
		int NumUVSets = 1;

		Ar << V.Pos;

		Ar << V.Normal[0] << V.Normal[1] << V.Normal[2];

		if (Ar.LegacyVer >= 709) NumUVSets = 4;

		for (int i = 0; i < NumUVSets; i++) Ar << V.UV[i];

		if (Ar.LegacyVer >= 710) Ar << V.Color;

		Ar << V.BoneIndex;
		return Ar;
	}
};


struct FLegacySoftVertex3
{
	FVector3f Pos;
	FPackedNormal Normal[3];
	FLegacyMeshUVFloat UV[NUM_UV_SETS_UE3];
	uint8 BoneIndex[NUM_INFLUENCES_UE3];
	uint8 BoneWeight[NUM_INFLUENCES_UE3];
	int32 Color;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacySoftVertex3& V)
	{
		int32 NumUVSets = 1;
		Ar << V.Pos;
		Ar << V.Normal[0] << V.Normal[1] << V.Normal[2];
		if (Ar.LegacyVer >= 709) NumUVSets = 4;
		for (int i = 0; i < NumUVSets; i++) Ar << V.UV[i];

		if (Ar.LegacyVer >= 710) Ar << V.Color;
		if (Ar.LegacyVer >= 333)
		{
			for (int32 i = 0; i < NUM_INFLUENCES_UE3; i++) Ar << V.BoneIndex[i];
			for (int32 i = 0; i < NUM_INFLUENCES_UE3; i++) Ar << V.BoneWeight[i];
		}
		else
		{
			for (int32 i = 0; i < NUM_INFLUENCES_UE3; i++) Ar << V.BoneIndex[i] << V.BoneWeight[i];
		}
		return Ar;
	}
};

struct FLegacySkeletalMeshChunk3
{
	int32 FirstVertex;
	TArray<FLegacyRigidVertex3> RigidVerts;
	TArray<FLegacySoftVertex3> SoftVerts;
	TArray<int16> Bones;
	int32 NumRigidVerts;
	int32 NumSoftVerts;
	int32 MaxInfluences;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacySkeletalMeshChunk3& V)
	{
		Ar << V.FirstVertex;
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
	TArray<uint16> Indices16;
	TArray<uint32> Indices32;

	FORCEINLINE bool Is32Bit() const
	{
		return (Indices32.Num() != 0);
	}

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacySkeletalMeshIndexBuffer3& I)
	{
		uint8 ItemSize = 2;

		if (Ar.LegacyVer >= 806)
		{
			int32 f0;
			Ar << f0 << ItemSize;
		}
		if (ItemSize == 2)
		{
			Ar.LegacyBulkSerialize(I.Indices16);
		}
		else
		{
			check(ItemSize == 4);
			Ar.LegacyBulkSerialize(I.Indices32);
		}

		int32 unk;
		if (Ar.LegacyVer < 297) Ar << unk;

		return Ar;
	}
};


// Structure holding normals and bone influences
struct FLegacyGPUVert3Common
{
	FPackedNormal Normal[3];
	uint8 BoneIndex[NUM_INFLUENCES_UE3];
	uint8 BoneWeight[NUM_INFLUENCES_UE3];
	static int32 GLegacyNumGPUUVSets;

	void Serialize(FRedUELegacyArchive& Ar)
	{
		if (Ar.LegacyVer < 494)
		{
			Ar << Normal[0] << Normal[1] << Normal[2];
		}
		else
		{
			Ar << Normal[0] << Normal[2];
		}
		int32 i;
		for (i = 0; i < NUM_INFLUENCES_UE3; i++) Ar << BoneIndex[i];
		for (i = 0; i < NUM_INFLUENCES_UE3; i++) Ar << BoneWeight[i];
	}
};


struct FLegacyGPUVert3Float : FLegacyGPUVert3Common
{
	FVector3f Pos;
	FLegacyMeshUVFloat UV[NUM_UV_SETS_UE3];

	FLegacyGPUVert3Float& operator=(const FLegacySoftVertex3& S)
	{
		int i;
		Pos = S.Pos;
		Normal[0] = S.Normal[0];
		Normal[1] = S.Normal[1];
		Normal[2] = S.Normal[2];
		for (i = 0; i < NUM_UV_SETS_UE3; i++) UV[i] = S.UV[i];
		for (i = 0; i < NUM_INFLUENCES_UE3; i++)
		{
			BoneIndex[i] = S.BoneIndex[i];
			BoneWeight[i] = S.BoneWeight[i];
		}
		return *this;
	}

	friend FArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyGPUVert3Float& V)
	{
		if (Ar.LegacyVer < 592) Ar << V.Pos;
		V.Serialize(Ar);
		if (Ar.LegacyVer >= 592) Ar << V.Pos;
		for (int i = 0; i < GLegacyNumGPUUVSets; i++) Ar << V.UV[i];
		return Ar;
	}
};

struct FLegacyVectorIntervalFixed32GPU
{
	int X : 11, Y : 11, Z : 10;

	FVector3f ToVector(const FVector3f& Mins, const FVector3f& Ranges) const
	{
		FVector3f r;
		r.X = (X / 1023.0f) * Ranges.X + Mins.X;
		r.Y = (Y / 1023.0f) * Ranges.Y + Mins.Y;
		r.Z = (Z / 511.0f) * Ranges.Z + Mins.Z;
		return r;
	}

	friend FArchive& operator<<(FArchive& Ar, FLegacyVectorIntervalFixed32GPU& V)
	{
		return Ar << *reinterpret_cast<uint32*>(&V);
	}
};

struct FLegacyGPUVert3Half : FLegacyGPUVert3Common
{
	FVector3f Pos;
	FLegacyMeshUVHalf UV[NUM_UV_SETS_UE3];

	friend FArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyGPUVert3Half& V)
	{
		if (Ar.LegacyVer < 592) Ar << V.Pos;
		V.Serialize(Ar);
		if (Ar.LegacyVer >= 592) Ar << V.Pos;
		for (int i = 0; i < GLegacyNumGPUUVSets; i++) Ar << V.UV[i];
		return Ar;
	}
};

struct FLegacyGPUVert3PackedHalf : FLegacyGPUVert3Common
{
	FLegacyVectorIntervalFixed32GPU Pos;
	FLegacyMeshUVHalf UV[NUM_UV_SETS_UE3];

	friend FArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyGPUVert3PackedHalf& V)
	{
		V.Serialize(Ar);
		Ar << V.Pos;
		for (int i = 0; i < GLegacyNumGPUUVSets; i++) Ar << V.UV[i];
		return Ar;
	}
};

struct FLegacyGPUVert3PackedFloat : FLegacyGPUVert3Common
{
	FLegacyVectorIntervalFixed32GPU Pos;
	FLegacyMeshUVFloat UV[NUM_UV_SETS_UE3];

	friend FArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyGPUVert3PackedFloat& V)
	{
		V.Serialize(Ar);
		Ar << V.Pos;
		for (int i = 0; i < GLegacyNumGPUUVSets; i++) Ar << V.UV[i];
		return Ar;
	}
};


struct FLegacySkeletalMeshVertexBuffer3
{
	int NumUVSets;
	int bUseFullPrecisionUVs; // 0 = half, 1 = float; copy of corresponding USkeletalMesh field
	// compressed position data
	int bUsePackedPosition; // 1 = packed FVector (32-bit), 0 = FVector (96-bit)
	FVector3f MeshOrigin;
	FVector3f MeshExtension;
	// vertex sets
	TArray<FLegacyGPUVert3Half> VertsHalf; // only one of these vertex sets are used
	TArray<FLegacyGPUVert3Float> VertsFloat;
	TArray<FLegacyGPUVert3PackedHalf> VertsHalfPacked;
	TArray<FLegacyGPUVert3PackedFloat> VertsFloatPacked;

	int32 GetVertexCount() const
	{
		if (VertsHalf.Num()) return VertsHalf.Num();
		if (VertsFloat.Num()) return VertsFloat.Num();
		if (VertsHalfPacked.Num()) return VertsHalfPacked.Num();
		if (VertsFloatPacked.Num()) return VertsFloatPacked.Num();
		return 0;
	}

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacySkeletalMeshVertexBuffer3& S)
	{
		S.bUsePackedPosition = false;
		bool AllowPackedPosition = false;
		S.NumUVSets = FLegacyGPUVert3Common::GLegacyNumGPUUVSets = 1;


		if (Ar.LegacyVer < 493)
		{
			// old version - FSoftVertex3 array
			TArray<FLegacySoftVertex3> Verts;
			Ar.LegacyBulkSerialize(Verts);
			// convert verts
			S.VertsFloat.SetNum(Verts.Num());
			for (int i = 0; i < Verts.Num(); i++)
			{
				S.VertsFloat[i] = Verts[i];
			}
			
			S.bUseFullPrecisionUVs = true;
			return Ar;
		}

		if (Ar.LegacyVer >= 709)
		{
			Ar << S.NumUVSets;
			FLegacyGPUVert3Common::GLegacyNumGPUUVSets = S.NumUVSets;
		}
		Ar << S.bUseFullPrecisionUVs;
		if (Ar.LegacyVer >= 592) Ar << S.bUsePackedPosition << S.MeshExtension << S.MeshOrigin;


		if (!AllowPackedPosition) S.bUsePackedPosition = false; // not used in games (see comment above)

		// serialize vertex array
		if (!S.bUseFullPrecisionUVs)
		{
		
			if (!S.bUsePackedPosition) Ar.LegacyBulkSerialize( S.VertsHalf);
			else  Ar.LegacyBulkSerialize( S.VertsHalfPacked);
		}
		else
		{
			if (!S.bUsePackedPosition) Ar.LegacyBulkSerialize( S.VertsFloat); 
			else  Ar.LegacyBulkSerialize( S.VertsFloatPacked); 
		}


		return Ar;
	}
};

struct FLegacyVertexInfluence
{
	int Weights;
	int Boned;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyVertexInfluence& S)
	{
		Ar << S.Weights << S.Boned;
		return Ar;
	}
};

struct FLegacyVertexInfluenceMapOld
{
	// key
	int f0;
	int f4;
	// value
	TArray<uint16> f8;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyVertexInfluenceMapOld& S)
	{
		Ar << S.f0 << S.f4 << S.f8;
		return Ar;
	}
};

struct FLegacyVertexInfluenceMap
{
	// key
	int f0;
	int f4;
	// value
	TArray<int> f8;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyVertexInfluenceMap& S)
	{
		Ar << S.f0 << S.f4 << S.f8;
		return Ar;
	}
};

struct FLegacySkeletalMeshVertexInfluences
{
	TArray<FLegacyVertexInfluence> Influences;
	TArray<FLegacyVertexInfluenceMap> VertexInfluenceMapping;
	TArray<FLegacySkeletalMeshSection3> Sections;
	TArray<FLegacySkeletalMeshChunk3> Chunks;
	TArray<uint8> RequiredBones;
	uint8 Usage; // default = 0

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacySkeletalMeshVertexInfluences& S)
	{
		Ar << S.Influences;
		if (Ar.LegacyVer >= 609)
		{
			if (Ar.LegacyVer >= 808)
			{
				Ar << S.VertexInfluenceMapping;
			}
			else
			{
				uint8 unk1;
				if (Ar.LegacyVer >= 806) Ar << unk1;
				TArray<FLegacyVertexInfluenceMapOld> VertexInfluenceMappingOld;
				Ar << VertexInfluenceMappingOld;
			}
		}
		if (Ar.LegacyVer >= 700)
		{
			Ar << S.Sections << S.Chunks;
		}
		if (Ar.LegacyVer >= 708)
		{
			Ar << S.RequiredBones;
		}
		if (Ar.LegacyVer >= 715)
		{
			Ar << S.Usage;
		}
		return Ar;
	}
};

struct FLegacySkeletalMeshLODModel3
{
	TArray<FLegacySkeletalMeshSection3> Sections;
	TArray<FLegacySkeletalMeshChunk3> Chunks;
	FLegacySkeletalMeshIndexBuffer3 IndexBuffer;
	TArray<int16> UsedBones;
	TArray<uint16> f68;
	TArray<uint8> f24;
	TArray<uint8> f74;
	int f80;
	int NumVertices;
	TArray<FLegacyEdge3> Edges;
	FRedUELegacyWordBulkData BulkData; // ElementCount = NumVertices
	FRedUELegacyIntBulkData BulkData2; // used instead of BulkData since version 806, indices?
	FLegacySkeletalMeshVertexBuffer3 GPUSkin;
	TArray<FLegacySkeletalMeshVertexInfluences> ExtraVertexInfluences; // GoW2+ engine
	int NumUVSets;
	TArray<FColor> VertexColor; // since version 710

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacySkeletalMeshLODModel3& Lod)
	{
		Ar << Lod.Sections << Lod.IndexBuffer;
		if (Ar.LegacyVer < 686) Ar << Lod.f68;
		Ar << Lod.UsedBones;
		if (Ar.LegacyVer < 686) Ar << Lod.f74;
		if (Ar.LegacyVer >= 215)
		{
			Ar << Lod.Chunks << Lod.f80 << Lod.NumVertices;
		}

		if (Ar.LegacyVer < 686) Ar << Lod.Edges;
		if (Ar.LegacyVer >= 207)
		{
			Ar << Lod.f24;
		}
		else
		{
			TArray<int16> f24_a;
			Ar << f24_a;
		}
		if (Ar.LegacyVer >= 221)
		{
			if (Ar.LegacyVer < 806) Lod.BulkData.Serialize(Ar); // Bulk of uint16
			else Lod.BulkData2.Serialize(Ar); // Bulk of int
		}
		if (Ar.Game == ERedUELegacyGame::Bioshock3)
		{
			int unkE4;
			Ar << unkE4;
		}

		Lod.NumUVSets = 1;
		if (Ar.LegacyVer >= 709)
		{
			Ar << Lod.NumUVSets;
		}
		if (Ar.LegacyVer >= 333)
		{
			Ar << Lod.GPUSkin;
		}
		if (Ar.LegacyVer >= 534) // post-UT3 code
		{
			Ar << Lod.ExtraVertexInfluences;
		}
		if (Ar.LegacyVer >= 841) // adjacency index buffer
		{
			FLegacySkeletalMeshIndexBuffer3 unk;
			Ar << unk;
		}
		return Ar;
	}
};

struct VLegacyJointPos
{
	FQuat4f Orientation;
	FVector3f Position;
	float Length = 1;
	FVector3f Size;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, VLegacyJointPos& P)
	{
		if (Ar.LegacyVer >= 224)
		{
			Ar << P.Orientation << P.Position;
			return Ar;
		}
		Ar << P.Orientation << P.Position << P.Length << P.Size;
		return Ar;
	}
};

struct FLegacyMeshBone
{
	FName Name;
	unsigned Flags;
	VLegacyJointPos BonePos;
	int ParentIndex; // 0 if this is the root bone.
	int NumChildren;

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyMeshBone& B)
	{
		Ar << B.Name << B.Flags;
		Ar << B.BonePos << B.NumChildren << B.ParentIndex;
		if (Ar.LegacyVer >= 515)
		{
			int unk44; // byte[4] ? default is 0xFF x 4
			Ar << unk44;
		}
		return Ar;
	}
};
namespace  SkeletalMeshImportData
{
	struct FBone;
}
class FSkeletalMeshImportData;

UCLASS()
class REDUELEGACY_API ULegacySkeletalMesh3 : public ULegacyObject
{
	GENERATED_BODY()

public:
											ULegacySkeletalMesh3				();
	virtual 								~ULegacySkeletalMesh3				() override;
	virtual FName							GetLegacyClassName_Implementation	(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual bool							LegacySupport_Implementation		(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
	virtual void							LegacySerialize						(FRedUELegacyArchive& Ar) override;
	virtual UObject*						ExportToContent						() override;

	FVector3f								MeshOrigin;
	FLegacyRotator							RotOrigin;
private:
			bool 							FindOrAddSkeleton					(USkeletalMesh* SkeletalMesh, const FString& String);
			bool 							CreateSkeletalMesh					(USkeletalMesh* SkeletalMesh,TArray<FSkeletalMeshImportData>& Array,const TArray<SkeletalMeshImportData::FBone>& Bones, const FString& String);
	FBoxSphereBounds3f						Bounds;
	TArray<FLegacyMeshBone> 				RefSkeleton;
	int32									SkeletalDepth;
	TArray<FLegacySkeletalMeshLODModel3>	LODModels;
public:
	UPROPERTY(Transient)
	TArray<ULegacyMaterialInterface*> Materials;
};
