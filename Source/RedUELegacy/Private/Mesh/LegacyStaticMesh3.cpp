#include "Mesh/LegacyStaticMesh3.h"

#include "PackageTools.h"
#include "StaticMeshAttributes.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacyGame.h"
#include "Material/LegacyMaterialInterface.h"


ULegacyStaticMesh3::ULegacyStaticMesh3()
{
}

ULegacyStaticMesh3::~ULegacyStaticMesh3()
{
}

FName ULegacyStaticMesh3::GetLegacyClassName_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
    static FName NAME_StaticMesh = "StaticMesh";
    return NAME_StaticMesh;
}

bool ULegacyStaticMesh3::LegacySupport_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
    if(EngineType != ERedUELegacyEngineType::UnrealEngine3)
    {
        return false;
    }
    return Super::LegacySupport_Implementation(EngineType, GameType);
}

struct FLegacyStaticMeshUnk5
{
    int					f0;
    uint8				f4[3];

    friend FArchive& operator<<(FArchive &Ar, FLegacyStaticMeshUnk5 &S)
    {
        return Ar << S.f0 << S.f4[0] << S.f4[1] << S.f4[2];
    }
};

void ULegacyStaticMesh3::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar << Bounds.Origin << Bounds.BoxExtent << Bounds.SphereRadius << BodySetup;
    if (Ar.LegacyVer < 315)
    {
        UObject *unk;
        Ar << unk;
    }
    if (Ar.Game == ERedUELegacyGame::Singularity)
    {
        // serialize kDOP tree
        check(Ar.LegacyLicenseeVer >= 112);
        // old serialization code
        Ar.LegacyBulkSerialize(kDOPNodes);
        Ar.LegacyBulkSerialize(kDOPTriangles);
        // new serialization code
        // bug in Singularity serialization code: serialized the same things twice!
    }
    if (Ar.Game != ERedUELegacyGame::Singularity&&Ar.LegacyVer < 770)
    {
        Ar.LegacyBulkSerialize(kDOPNodes);
    }
    else
    {
        FLegacykDOPBounds kDOPBounds;
        TArray<FLegacykDOPNode3New> Nodes;
        Ar << kDOPBounds;
        Ar.LegacyBulkSerialize(Nodes);
    }
    Ar.LegacyBulkSerialize(kDOPTriangles);
    
    Ar << InternalVersion;
    if (InternalVersion >= 17 && Ar.LegacyVer < 593)
    {
        TArray<FName> IgnoredLegacyContentTags;			// some text properties; ContentTags ? (switched from binary to properties)
        Ar << IgnoredLegacyContentTags;
    }
    if (Ar.LegacyVer >= 823)
    {
        int32 unkFlag;
        FLegacyStaticMeshLODModel3 unkLod;
        Ar << unkFlag;
        if (unkFlag)
        {
            Ar << unkLod;
        }
    
        if (Ar.LegacyVer < 829)
        {
            TArray<int32> unk;
            Ar << unk;
        }
        else
        {
            TArray<FLegacyStaticMeshUnk5> f178;
            Ar << f178;
        }
        int32 f74;
        Ar << f74;
    }
    if (Ar.LegacyVer >= 859)
    {
        int32 unk;
        Ar << unk;
    }
    if (Ar.Game == ERedUELegacyGame::Singularity)
    {
        int Count;
        Ar << Count;
        if (Count)
        {
            Lods.AddZeroed();
            Ar << Lods[0];
        }
    }
    else
    {
        Ar << Lods;
    }
    Ar.Seek(Ar.GetStopper());
}

UObject* ULegacyStaticMesh3::ExportToContent()
{
	if(PresentObject)
	{
		return PresentObject;
	}
    const FString ObjectPath = GetOutContentPath() / GetLegacyFullName().Replace(TEXT("."),TEXT("/"));
    const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
    const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
    UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *FullObjectPath,nullptr,LOAD_NoWarn);
    if(!StaticMesh)
    {
        
        UPackage*  AssetPackage = CreatePackage(*PackageName);
        StaticMesh = NewObject<UStaticMesh>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public|RF_Standalone);
        StaticMesh->PreEditChange(nullptr);
        FAssetRegistryModule::AssetCreated(StaticMesh);
        TArray<FStaticMaterial> Materials;
        for (int32 LodIndex = 0; LodIndex < Lods.Num(); LodIndex++)
        {
            FStaticMeshSourceModel& SourceModel = StaticMesh->AddSourceModel();
            FMeshDescription* MeshDescription = StaticMesh->CreateMeshDescription(LodIndex);
            if (MeshDescription)
            {
                FStaticMeshAttributes StaticMeshAttributes(*MeshDescription);
                TVertexAttributesRef<FVector3f> VertexPositions = StaticMeshAttributes.GetVertexPositions();
                TEdgeAttributesRef<bool>  EdgeHardnesses = StaticMeshAttributes.GetEdgeHardnesses();
                TPolygonGroupAttributesRef<FName> PolygonGroupImportedMaterialSlotNames = StaticMeshAttributes.GetPolygonGroupMaterialSlotNames();
                TVertexInstanceAttributesRef<FVector3f> VertexInstanceNormals = StaticMeshAttributes.GetVertexInstanceNormals();
                TVertexInstanceAttributesRef<FVector3f> VertexInstanceTangents = StaticMeshAttributes.GetVertexInstanceTangents();
                TVertexInstanceAttributesRef<float> VertexInstanceBinormalSigns = StaticMeshAttributes.GetVertexInstanceBinormalSigns();
                TVertexInstanceAttributesRef<FVector2f> VertexInstanceUVs = StaticMeshAttributes.GetVertexInstanceUVs();
                TVertexInstanceAttributesRef<FVector4f> VertexInstanceColors = StaticMeshAttributes.GetVertexInstanceColors();
                VertexInstanceUVs.SetNumChannels( Lods[LodIndex].UVStream.NumTexCoords);
                for (size_t ElementID = 0; ElementID < Lods[LodIndex].Sections.Num(); ElementID++)
                {
                
                    const FPolygonGroupID CurrentPolygonGroupID = MeshDescription->CreatePolygonGroup();
					PolygonGroupImportedMaterialSlotNames[CurrentPolygonGroupID] = *FString::Printf(TEXT("mat_%d"), Materials.Num());
                    const uint16* Indices = Lods[LodIndex].Indices.Indices.GetData() + Lods[LodIndex].Sections[ElementID].FirstIndex;
					for (size_t FaceID = 0; FaceID < Lods[LodIndex].Sections[ElementID].NumFaces; FaceID++)
					{
						FVertexID VertexIDs[3];
						VertexIDs[0] = MeshDescription->CreateVertex();
						VertexIDs[1] = MeshDescription->CreateVertex();
						VertexIDs[2] = MeshDescription->CreateVertex();
					    
					    VertexPositions[VertexIDs[0]] = Lods[LodIndex].VertexStream.Verts[Indices[FaceID * 3 + 0]];
					    VertexPositions[VertexIDs[1]] = Lods[LodIndex].VertexStream.Verts[Indices[FaceID * 3 + 1]];
					    VertexPositions[VertexIDs[2]] = Lods[LodIndex].VertexStream.Verts[Indices[FaceID * 3 + 2]];
					    
						TArray<FVertexInstanceID> VertexInstanceIDs;

						VertexInstanceIDs.SetNum(3);

						for (int32 CornerIndex = 0; CornerIndex < 3; ++CornerIndex)
						{
							VertexInstanceIDs[CornerIndex] = MeshDescription->CreateVertexInstance(VertexIDs[CornerIndex]);;
						}
                        for(int32 UVId = 0;UVId<Lods[LodIndex].UVStream.NumTexCoords;UVId++)
                        {
                            for(int32 VertexID = 0;VertexID<3;VertexID++)
                            {
                                FLegacyMeshUVFloat InUV = Lods[LodIndex].UVStream.UV[Indices[FaceID * 3 + VertexID]].UV[UVId];
                                VertexInstanceUVs.Set(VertexInstanceIDs[VertexID], UVId, {InUV.U,InUV.V});
                            }
                        }
					    for(int32 VertexID = 0;VertexID<3;VertexID++)
					    {
					        auto ToVector= [](const FPackedNormal&Normal)->FVector3f
					        {
					           FVector3f Result ;
				               Result.X = ( Normal.Vector.Packed        & 0xFF) / 127.5f - 1;
				               Result.Y = (( Normal.Vector.Packed  >> 8 ) & 0xFF) / 127.5f - 1;
				               Result.Z = (( Normal.Vector.Packed  >> 16) & 0xFF) / 127.5f - 1;
					           return Result;
					        };
					        FVector3f Normal = ToVector(Lods[LodIndex].UVStream.UV[Indices[FaceID * 3 + VertexID]].Normal[2]);
					        FVector3f Binormal = ToVector(Lods[LodIndex].UVStream.UV[Indices[FaceID * 3 + VertexID]].Normal[1]);
					        FVector3f Tangent = ToVector(Lods[LodIndex].UVStream.UV[Indices[FaceID * 3 + VertexID]].Normal[0].ToFVector3f());
					        
					        VertexInstanceNormals[VertexInstanceIDs[VertexID]] = Normal;
					        VertexInstanceTangents[VertexInstanceIDs[VertexID]] = Tangent;
					        if (Lods[LodIndex].UVStream.UV[Indices[FaceID * 3 + VertexID]].Normal[1].Vector.Packed != 0)
					        {
					            VertexInstanceBinormalSigns[VertexInstanceIDs[VertexID]] =  Binormal.Dot( FVector3f::CrossProduct(Normal,Tangent))<0.f?-1.f:1.f;
					        }
                            else
                            {
                                VertexInstanceBinormalSigns[VertexInstanceIDs[VertexID]] =  1.f;
                            }
					        VertexInstanceColors[VertexInstanceIDs[VertexID]] = FLinearColor(Lods[LodIndex].UVStream.UV[Indices[FaceID * 3 + VertexID]].Color);
					    }
					    
						TArray<FEdgeID> NewEdgeIDs;
						const FPolygonID NewPolygonID = MeshDescription->CreatePolygon(CurrentPolygonGroupID, VertexInstanceIDs, &NewEdgeIDs);
						for (const FEdgeID& NewEdgeID : NewEdgeIDs)
						{
							//All edge are hard for BSP
							EdgeHardnesses[NewEdgeID] = true;
						}
					}
                	UMaterialInterface* MaterialInterface = nullptr;
                	if(Lods[LodIndex].Sections[ElementID].Mat)
                	{
                		MaterialInterface = CastChecked<UMaterialInterface>(Lods[LodIndex].Sections[ElementID].Mat->ExportToContent(),ECastCheckedType::NullAllowed);
                	}
                    //CreateMaterial(ImporterStaticMesh->GetIndexMaterial(ElementID, LodIndex), ParentPackage, Flags, Options)
					Materials.AddUnique(FStaticMaterial(MaterialInterface, *FString::Printf(TEXT("mat_%d"),Materials.Num()), *FString::Printf(TEXT("mat_%d"), Materials.Num())));
                }
            }
            StaticMesh->CommitMeshDescription(LodIndex++);
            SourceModel.BuildSettings.bRecomputeNormals = false;
            SourceModel.BuildSettings.bGenerateLightmapUVs = true;
            SourceModel.BuildSettings.DstLightmapIndex = 1;
            SourceModel.BuildSettings.MinLightmapResolution = 128;
        }
        StaticMesh->SetStaticMaterials(Materials);
        StaticMesh->Build();
        StaticMesh->PostEditChange();
        StaticMesh->Modify();
        
    }
	PresentObject = StaticMesh;
    return StaticMesh;
    
}
