#pragma once

#include "Core/LegacyObject.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacyArchive.h"
#include "Material/LegacyMaterialInterface.h"
#include "LegacyStaticMesh3.generated.h"


struct FLegacykDOPBounds
{
    FVector3f			v1;
    FVector3f			v2;

    friend FArchive& operator<<(FArchive &Ar, FLegacykDOPBounds &V)
    {
        return Ar << V.v1 << V.v2;
    }
};


struct FLegacykDOPNode3
{
    FLegacykDOPBounds	Bounds;
    int					f18;
    int16				f1C;
    int16				f1E;

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacykDOPNode3 &V)
    {
        if ((Ar.LegacyVer < 209) || (Ar.LegacyVer >= 468))
        {
            Ar << V.f1C << V.f1E;
        }
        else
        {
            int tmp1C, tmp1E;
            Ar << tmp1C << tmp1E;
            V.f1C = tmp1C;
            V.f1E = tmp1E;
        }
        return Ar;
    }
};




struct FLegacykDOPTriangle3
{
    int16				f0, f2, f4, f6;
    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacykDOPTriangle3 &V)
    {
        if ((Ar.LegacyVer < 209) || (Ar.LegacyVer >= 468))
        {
            Ar << V.f0 << V.f2 << V.f4 << V.f6;
        }
        else
        {
            int tmp0, tmp2, tmp4, tmp6;
            Ar << tmp0 << tmp2 << tmp4 << tmp6;
            V.f0 = tmp0;
            V.f2 = tmp2;
            V.f4 = tmp4;
            V.f6 = tmp6;
        }
        return Ar;
    }
};

struct FLegacykDOPNode3New
{
    uint8				mins[3];
    uint8				maxs[3];

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacykDOPNode3New &V)
    {
        Ar << V.mins[0] << V.mins[1] << V.mins[2] << V.maxs[0] << V.maxs[1] << V.maxs[2];
        return Ar;
    }
};


struct FLegacyPS3StaticMeshData
{
    TArray<int>			unk1;
    TArray<int>			unk2;
    TArray<uint16>		unk3;
    TArray<uint16>		unk4;
    TArray<uint16>		unk5;
    TArray<uint16>		unk6;
    TArray<uint16>		unk7;
    TArray<uint16>		unk8;

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyPS3StaticMeshData &S)
    {
        Ar << S.unk1 << S.unk2 << S.unk3 << S.unk4 << S.unk5 << S.unk6 << S.unk7 << S.unk8;
        return Ar;
    }
};

struct FLegacyStaticMeshSection3
{
    ULegacyMaterialInterface* Mat;
    int32				            f10;		//?? bUseSimple...Collision
    int32				            f14;		//?? ...
    int32				            bEnableShadowCasting;
    int32				            FirstIndex;
    int32				            NumFaces;
    int32				            f24;		//?? first used vertex
    int32				            f28;		//?? last used vertex
    int32				            Index;		//?? index of section
    TArray<int64>                   f30;

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyStaticMeshSection3 &S)
    {
        Ar << S.Mat << S.f10 << S.f14;
        if (Ar.LegacyVer >= 473) Ar << S.bEnableShadowCasting;
        Ar << S.FirstIndex << S.NumFaces << S.f24 << S.f28;
        if (Ar.LegacyVer >= 492) Ar << S.Index;
		if (Ar.LegacyVer >= 514) Ar << S.f30;
        if (Ar.Game == ERedUELegacyGame::Bioshock3)
        {
            FString unk4;
            Ar << unk4;
        }
        if (Ar.LegacyVer >= 618)
        {
            uint8 unk;
            Ar << unk;
            if (unk)
            {
                FLegacyPS3StaticMeshData ps3data;
                Ar << ps3data;
            }
        }
        return Ar;
    }
};



struct FLegacyVectorIntervalFixed48Bio
{
    uint16			X, Y, Z;

    FVector3f ToVector(const FVector3f &Mins, const FVector3f &Ranges) const
    {
        FVector3f r;
        r.X = (X / 65535.0f) * Ranges.X + Mins.X;
        r.Y = (Y / 65535.0f) * Ranges.Y + Mins.Y;
        r.Z = (Z / 65535.0f) * Ranges.Z + Mins.Z;
        return r;
    }

    friend FArchive& operator<<(FArchive &Ar, FLegacyVectorIntervalFixed48Bio &V)
    {
        return Ar << V.X << V.Y << V.Z;
    }
};


struct FLegacyVectorIntervalFixed64
{
    int16			X, Y, Z, W;

    FVector3f ToVector(const FVector3f &Mins, const FVector3f &Ranges) const
    {
        FVector3f r;
        r.X = (X / 32767.0f) * Ranges.X + Mins.X;
        r.Y = (Y / 32767.0f) * Ranges.Y + Mins.Y;
        r.Z = (Z / 32767.0f) * Ranges.Z + Mins.Z;
        return r;
    }

    friend FArchive& operator<<(FArchive &Ar, FLegacyVectorIntervalFixed64 &V)
    {
        return Ar << V.X << V.Y << V.Z << V.W;
    }
};


struct FLegacyStaticMeshVertexStream3
{
    int					    VertexSize;		// 0xC
    int					    NumVerts;		// == Verts.Num()
    TArray<FVector3f>		Verts;
    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyStaticMeshVertexStream3 &S)
    {
        
        Ar << S.VertexSize << S.NumVerts;
        if (Ar.Game == ERedUELegacyGame::Bioshock3)
        {
            uint8 IsPacked, VectorType;		// VectorType used only when IsPacked != 0
            FVector3f Mins, Extents;
            Ar << IsPacked << VectorType << Mins << Extents;
            if (IsPacked)
            {
                if (VectorType)
                {
                    TArray<FLegacyVectorIntervalFixed48Bio> Vecs16x3;
                    Ar.LegacyBulkSerialize(Vecs16x3);
                    S.Verts.AddUninitialized(Vecs16x3.Num());
                    for (int i = 0; i < Vecs16x3.Num(); i++)
                        S.Verts[i] = Vecs16x3[i].ToVector(Mins, Extents);
                }
                else
                {
                    TArray<FLegacyVectorIntervalFixed64> Vecs16x4;
                    Ar.LegacyBulkSerialize(Vecs16x4);
                    S.Verts.AddUninitialized(Vecs16x4.Num());
                    for (int i = 0; i < Vecs16x4.Num(); i++)
                        S.Verts[i] = Vecs16x4[i].ToVector(Mins, Extents);
                }
                return Ar;
            }
            // else - normal vertex stream
        }
        Ar.LegacyBulkSerialize(S.Verts);
        return Ar;
    }
};

#define NUM_INFLUENCES_UE3			4
#define NUM_UV_SETS_UE3				4

struct FLegacyMeshUVFloat
{
    float			U;
    float			V;

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyMeshUVFloat &M)
    {
        Ar << M.U << M.V;
        return Ar;
    }
};

struct FLegacyMeshUVHalf
{
    uint16			U;
    uint16			V;

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyMeshUVHalf &V)
    {
        Ar << V.U << V.V;
        return Ar;
    }

    operator FLegacyMeshUVFloat() const
    {
        FLegacyMeshUVFloat r;
        r.U = FPlatformMath::LoadHalf(&U);
        r.V = FPlatformMath::LoadHalf(&V);
        return r;
    }
};


struct FLegacyStaticMeshUVItem3
{
    static bool GUseStaticFloatUVs;
    static int32 GNumStaticUVSets;
    
    FVector3f			Pos;			// old version (< 472)
    FPackedNormal		Normal[3];
    FColor				Color;
    FLegacyMeshUVFloat	UV[NUM_UV_SETS_UE3];

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyStaticMeshUVItem3 &V)
    {
        if (Ar.LegacyVer < 472)
        {
            // old version has position embedded into UVStream (this is not an UVStream, this is a single stream for everything)
            Ar << V.Pos << V.Color;
        }
        if (Ar.LegacyVer < 477)
            Ar << V.Normal[0] << V.Normal[1] << V.Normal[2];
        else
            Ar << V.Normal[0] << V.Normal[2];
        if (Ar.LegacyVer >= 434 && Ar.LegacyVer < 615)
            Ar << V.Color;				// starting from 615 made as separate stream
        
        if (GUseStaticFloatUVs)
        {
            for (int32 i = 0; i < GNumStaticUVSets; i++)
                Ar << V.UV[i];
        }
        else
        {
            for (int32 i = 0; i < GNumStaticUVSets; i++)
            {
                // read in half format and convert to float
                FLegacyMeshUVHalf UVHalf;
                Ar << UVHalf;
                V.UV[i] = UVHalf;		// convert
            }
        }
        return Ar;
    }
};
bool FLegacyStaticMeshUVItem3::GUseStaticFloatUVs = false;
int32 FLegacyStaticMeshUVItem3::GNumStaticUVSets = 0;

struct FLegacyStaticMeshUVStream3
{
    int32   			                NumTexCoords;
    int32   			                ItemSize;
    int32   			                NumVerts;
    int32   			                bUseFullPrecisionUVs;
    TArray<FLegacyStaticMeshUVItem3>    UV;
    
    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyStaticMeshUVStream3 &S)
    {
        Ar << S.NumTexCoords << S.ItemSize << S.NumVerts;
        S.bUseFullPrecisionUVs = true;
        if (Ar.LegacyVer >= 474)
        {
            Ar << S.bUseFullPrecisionUVs;
        }
        check( S.NumTexCoords <= 8);
        FLegacyStaticMeshUVItem3::GNumStaticUVSets   = S.NumTexCoords;
        FLegacyStaticMeshUVItem3::GUseStaticFloatUVs = (S.bUseFullPrecisionUVs != 0);
        Ar.LegacyBulkSerialize(S.UV);
        return Ar;
    }
};

struct FLegacyStaticMeshShadowVolumeStream3
{
    int32				ItemSize;
    int32				NumVerts;
    TArray<float>		Colors;

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyStaticMeshShadowVolumeStream3 &S)
    {
        Ar << S.ItemSize << S.NumVerts;
        Ar.LegacyBulkSerialize(S.Colors);
        return Ar;
    }
};

struct FLegacyStaticMeshColorStream3		// ArVer >= 615
{
    int32				ItemSize;
    int32				NumVerts;
    TArray<FColor>		Colors;

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyStaticMeshColorStream3 &S)
    {
        Ar << S.ItemSize << S.NumVerts;
        if (S.NumVerts)
        {
            Ar.LegacyBulkSerialize(S.Colors);
        }
        return Ar;
    };
};


struct FLegacyStaticMeshVertex3Old			// ArVer < 333
{
    FVector3f			Pos;
    FPackedNormal		Normal[3];		// packed vector

    operator FVector3f() const
    {
        return Pos;
    }

    friend FArchive& operator<<(FArchive &Ar, FLegacyStaticMeshVertex3Old &V)
    {
        return Ar << V.Pos << V.Normal[0] << V.Normal[1] << V.Normal[2];
    }
};


struct FLegacyStaticMeshUVStream3Old			// ArVer < 364; corresponds to UE2 StaticMesh?
{
    TArray<FLegacyMeshUVFloat> Data;

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar,FLegacyStaticMeshUVStream3Old &S)
    {
        int unk;						// Revision?
        Ar << S.Data;					// used BulkSerialize, but BulkSerialize is newer than this version
        if (Ar.LegacyVer < 297) Ar << unk;
        return Ar;
    }
};

struct FLegacyIndexBuffer3
{
    TArray<uint16>		Indices;
    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyIndexBuffer3 &I)
    {
        int32 unk;						// Revision?
        Ar.LegacyBulkSerialize(I.Indices);
        if (Ar.LegacyVer < 297) Ar << unk;	// at older version compatible with FRawIndexBuffer
        return Ar;
    }
};


struct FLegacyEdge3
{
    int32   iVertex[2];
    int32   iFace[2];

    friend FArchive& operator<<(FArchive &Ar, FLegacyEdge3 &V)
    {
        return Ar << V.iVertex[0] << V.iVertex[1] << V.iFace[0] << V.iFace[1];
    }
};

struct FLegacyStaticMeshLODModel3
{
    FRedUELegacyByteBulkData		        BulkData;		// ElementSize = 0xFC for UT3 and 0x170 for UDK ... it's simpler to skip it
    TArray<FLegacyStaticMeshSection3>       Sections;
    FLegacyStaticMeshVertexStream3          VertexStream;
    FLegacyStaticMeshUVStream3              UVStream;
    FLegacyStaticMeshShadowVolumeStream3    ShadowVolumeStream;
    FLegacyStaticMeshColorStream3           ColorStream;
    FLegacyIndexBuffer3		                Indices;
    FLegacyIndexBuffer3		                Indices2;		// wireframe
    int32					                NumVerts;
    TArray<FLegacyEdge3>		            Edges;
    TArray<uint8>		                    fEC;			// flags for faces? removed simultaneously with Edges

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyStaticMeshLODModel3 &Lod)
    {
        if (Ar.LegacyVer >= 218)
            Lod.BulkData.Skip(Ar);
        Ar << Lod.Sections;
        if (Ar.LegacyVer >= 472)
        {
            Ar << Lod.VertexStream;
            Ar << Lod.UVStream;
            if (Ar.LegacyVer >= 615)
            {
                Ar << Lod.ColorStream;
            }
            if (Ar.LegacyVer < 686) Ar << Lod.ShadowVolumeStream;
            Ar << Lod.NumVerts;
        }
        else if (Ar.LegacyVer >= 466)
        {
            Ar << Lod.VertexStream;
            Ar << Lod.UVStream;
            Ar << Lod.NumVerts;
        }
        else if (Ar.LegacyVer >= 364)
        {
            // ver_2:
            Ar << Lod.UVStream;
            Ar << Lod.NumVerts;
            // create VertexStream
            const int32 NumVerts = Lod.UVStream.UV.Num();
            Lod.VertexStream.Verts.Empty(NumVerts);
            //			Lod.VertexStream.NumVerts = NumVerts;
            for (int i = 0; i < NumVerts; i++)
                Lod.VertexStream.Verts.Add(Lod.UVStream.UV[i].Pos);
        }
        else
        {
            // ver_1:
            TArray<FLegacyStaticMeshUVStream3Old> UVStream;
            if (Ar.LegacyVer >= 333)
            {
                ensureMsgf(false,TEXT("StaticMesh: untested code! (LegacyVer=%d)"), Ar.LegacyVer);
                TArray<FQuat> Verts;
                TArray<int>   Normals;	// compressed
                Ar << Verts << Normals << UVStream;	// really used BulkSerialize, but it is too new for this code
                //!! convert
            }
            else
            {
                // oldest version
                TArray<FLegacyStaticMeshVertex3Old> Verts;
                Ar << Verts << UVStream;
                // convert vertex stream
                const int32 NumVerts     = Verts.Num();
                const int32 NumTexCoords = UVStream.Num();
                Lod.VertexStream.Verts.Empty(NumVerts);
                Lod.VertexStream.Verts.AddZeroed(NumVerts);
                Lod.UVStream.UV.Empty();
                Lod.UVStream.UV.AddDefaulted(NumVerts);
                Lod.UVStream.NumVerts     = NumVerts;
                Lod.UVStream.NumTexCoords = NumTexCoords;
                // resize UV streams
                for (int32 i = 0; i < NumVerts; i++)
                {
                    FLegacyStaticMeshVertex3Old &V = Verts[i];
                    FVector3f                   &DV = Lod.VertexStream.Verts[i];
                    FLegacyStaticMeshUVItem3    &UV = Lod.UVStream.UV[i];
                    DV           = V.Pos;
                    UV.Normal[2] = V.Normal[2];
                    for (int j = 0; j < NumTexCoords; j++)
                        UV.UV[j] = UVStream[j].Data[i];
                }
            }
        }
        
        Ar << Lod.Indices;
		Ar << Lod.Indices2;
        if (Ar.LegacyVer >= 841)
        {
            FLegacyIndexBuffer3 Indices3;
            Ar << Indices3;
        }
        return Ar;
    }
};

UCLASS()
class REDUELEGACY_API ULegacyStaticMesh3 : public ULegacyObject
{
    GENERATED_BODY()

public:
										ULegacyStaticMesh3					();
    virtual                             ~ULegacyStaticMesh3                 () override;
    virtual FName                       GetLegacyClassName_Implementation   (ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
    virtual bool                        LegacySupport_Implementation        (ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType) override;
    virtual void                        LegacySerialize                     (FRedUELegacyArchive& Ar) override;
    virtual UObject*                    ExportToContent                     () override;

    FBoxSphereBounds3f                  Bounds;
    TArray<FLegacyStaticMeshLODModel3>  Lods;
    TArray<FLegacykDOPNode3>		    kDOPNodes;
    TArray<FLegacykDOPTriangle3>	    kDOPTriangles;


    
    UPROPERTY(BlueprintReadWrite)
    FString SourceFilePath;

    UPROPERTY(BlueprintReadWrite)
    ULegacyObject* BodySetup = nullptr;

    UPROPERTY(BlueprintReadWrite)
    bool UseSimpleLineCollision = false;
    
    UPROPERTY(BlueprintReadWrite)
    bool UseSimpleBoxCollision = false;
    
    UPROPERTY(BlueprintReadWrite)
    bool UseSimpleRigidBodyCollision = false;
    
    UPROPERTY(BlueprintReadWrite)
    bool UseFullPrecisionUVs = false;
    
    UPROPERTY(BlueprintReadWrite)
    int32 InternalVersion = INDEX_NONE;

};
