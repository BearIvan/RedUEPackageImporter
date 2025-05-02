#include "Mesh/LegacySkeletalMesh3.h"

#include "Editor.h"
#include "MeshUtilities.h"
#include "PackageTools.h"
#include "SkeletalMeshTypes.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/RedUELegacySubsystem.h"
#include "Engine/SkinnedAssetCommon.h"
#include "ImportUtils/SkeletalMeshImportUtils.h"
#include "Interfaces/ITargetPlatformManagerModule.h"
#include "Rendering/SkeletalMeshLODImporterData.h"
#include "Rendering/SkeletalMeshModel.h"
int32 FLegacyGPUVert3Common::GLegacyNumGPUUVSets = 1;

ULegacySkeletalMesh3::ULegacySkeletalMesh3(): SkeletalDepth(0)
{
}

ULegacySkeletalMesh3::~ULegacySkeletalMesh3()
{
}

FName ULegacySkeletalMesh3::GetLegacyClassName_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	static FName NAME_SkeletalMesh = "SkeletalMesh";
	return NAME_SkeletalMesh;
}

bool ULegacySkeletalMesh3::LegacySupport_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
	if (EngineType != ERedUELegacyEngineType::UnrealEngine3)
	{
		return false;
	}
	return Super::LegacySupport_Implementation(EngineType, GameType);
}

void ULegacySkeletalMesh3::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	Ar << Bounds.Origin << Bounds.BoxExtent << Bounds.SphereRadius;
	Ar << Materials;
	Ar << MeshOrigin << RotOrigin;
	Ar << RefSkeleton << SkeletalDepth;
	Ar << LODModels;
}

struct FLegacySkeletalUV
{
	FVector2f UV[4] = {};
};
struct FLegacySkeletalNormal
{
	FVector3f Tangent;
	FVector3f Binormal;
	FVector3f Normal;
	
};
UObject* ULegacySkeletalMesh3::ExportToContent()
{
	if (PresentObject)
	{
		return PresentObject;
	}
	
	const FString ObjectPath = GetOutContentPath() / GetLegacyFullName().Replace(TEXT("."),TEXT("/"));
	const FString PackageName = UPackageTools::SanitizePackageName(ObjectPath);
	const FString FullObjectPath = PackageName + TEXT(".") + FPaths::GetBaseFilename(PackageName);
	USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, *FullObjectPath, nullptr, LOAD_NoWarn);
	if (!SkeletalMesh)
	{
		UPackage* AssetPackage = CreatePackage(*PackageName);
		SkeletalMesh = NewObject<USkeletalMesh>(AssetPackage, *FPaths::GetBaseFilename(PackageName), RF_Public | RF_Standalone);
		SkeletalMesh->PreEditChange(nullptr);
		FAssetRegistryModule::AssetCreated(SkeletalMesh);
		{
			TArray<FSkeletalMeshImportData> SkeletalMeshImportDataList;
			SkeletalMeshImportDataList.AddDefaulted(LODModels.Num());


			TArray<SkeletalMeshImportData::FBone> Bones;
			Bones.AddDefaulted(RefSkeleton.Num());
			for (int32 BoneIndex = 0; BoneIndex < RefSkeleton.Num(); BoneIndex++)
			{
				SkeletalMeshImportData::FBone& Bone = Bones[BoneIndex];
				Bone.Flags = 0;
				Bone.ParentIndex = INDEX_NONE;
				Bone.NumChildren = 0;
			}
			for (int32 BoneIndex = 0; BoneIndex < RefSkeleton.Num(); BoneIndex++)
			{
				SkeletalMeshImportData::FBone& Bone = Bones[BoneIndex];

				Bone.ParentIndex = RefSkeleton[BoneIndex].ParentIndex;

				if (BoneIndex == 0)
				{
					check(RefSkeleton[BoneIndex].ParentIndex == 0);
					Bone.ParentIndex = INDEX_NONE;
				}
				else
				{
					Bones[Bone.ParentIndex].NumChildren++;
				}


				FVector3f Location = RefSkeleton[BoneIndex].BonePos.Position;
				FQuat4f Orientation = RefSkeleton[BoneIndex].BonePos.Orientation;
				Bone.BonePos.Transform.SetRotation(Orientation);
				Bone.BonePos.Transform.SetLocation(Location);
				Bone.BonePos.Length = RefSkeleton[BoneIndex].BonePos.Length;
				Bone.BonePos.XSize = RefSkeleton[BoneIndex].BonePos.Size.X;
				Bone.BonePos.YSize = RefSkeleton[BoneIndex].BonePos.Size.Y;
				Bone.BonePos.ZSize = RefSkeleton[BoneIndex].BonePos.Size.Z;

				Bone.Name = RefSkeleton[BoneIndex].Name.ToString();
			}
			for (int32 LodIndex = 0; LodIndex < LODModels.Num(); LodIndex++)
			{
				
				int32 NumTexCoords = FMath::Max(LODModels[LodIndex].NumUVSets, LODModels[LodIndex].GPUSkin.NumUVSets);	
				FSkeletalMeshImportData& InSkeletalMeshImportData = SkeletalMeshImportDataList[LodIndex];
				InSkeletalMeshImportData.NumTexCoords = NumTexCoords;
				InSkeletalMeshImportData.bHasNormals = true;
				InSkeletalMeshImportData.bHasTangents = true;
				InSkeletalMeshImportData.bHasVertexColors = LODModels[LodIndex].VertexColor.Num()>0;
				InSkeletalMeshImportData.RefBonesBinary = Bones;
				
				int32 VertexCount = LODModels[LodIndex].GPUSkin.GetVertexCount();
				bool UseGpuSkinVerts = (VertexCount > 0);
				if (!VertexCount)
				{
					const FLegacySkeletalMeshChunk3& C = LODModels[LodIndex].Chunks.Last(); // last chunk
					VertexCount = C.FirstVertex + C.NumRigidVerts + C.NumSoftVerts;
				}

				TArray<FLegacySkeletalUV> UVs;
				TArray<FLegacySkeletalNormal> Normals;
				
				const FLegacySkeletalMeshChunk3 *SkeletalMeshChunk = nullptr;
				int32 ChunkIndex = 0;
				int32 LastChunkVertex = -1;
				for (int32 VertexID = 0; VertexID < VertexCount; VertexID++)
				{
					
					
					if (VertexID >= LastChunkVertex)
					{
						SkeletalMeshChunk = &LODModels[LodIndex].Chunks[ChunkIndex++];
						LastChunkVertex = SkeletalMeshChunk->FirstVertex + SkeletalMeshChunk->NumRigidVerts + SkeletalMeshChunk->NumSoftVerts;
					}
					FVector3f Vertex;
					FLegacySkeletalUV& OutUV = UVs.AddDefaulted_GetRef();
					FLegacySkeletalNormal OutNormal = Normals.AddDefaulted_GetRef();
					auto UnpackNormal = [&OutNormal](const FPackedNormal* InNormal)
					{
						// convert Normal[3]
						OutNormal.Normal = InNormal[2].ToFVector3f();
						OutNormal.Tangent = InNormal[0].ToFVector3f();
						if (InNormal[1].Vector.Packed != 0)
						{
							OutNormal.Binormal = InNormal[1].ToFVector3f();
						}
						else
						{
							FVector3f Tangent  = InNormal[0].ToFVector3f();
							FVector3f Normal   = InNormal[2].ToFVector3f();
							FVector3f ComputedBinormal = FVector3f::CrossProduct(Normal,Tangent);
							OutNormal.Binormal = ComputedBinormal;
						}
					};
					
					const FLegacySkeletalMeshVertexBuffer3 &S = LODModels[LodIndex].GPUSkin;
					if (UseGpuSkinVerts)
					{
						// NOTE: Gears3 has some issues:
						// - chunk may have FirstVertex set to incorrect value (for recent UE3 versions), which overlaps with the
						//   previous chunk (FirstVertex=0 for a few chunks)
						// - index count may be greater than sum of all face counts * 3 from all mesh sections -- this is verified in PSK exporter

						// get vertex from GPU skin
						const FLegacyGPUVert3Common* V; // has normal and influences, but no UV[] and position

						if (!S.bUseFullPrecisionUVs)
						{
							// position
							const FLegacyMeshUVHalf* SUV;
							if (!S.bUsePackedPosition)
							{
								const FLegacyGPUVert3Half& V0 = S.VertsHalf[VertexID];
								Vertex = V0.Pos;
								V = &V0;
								SUV = V0.UV;
							}
							else
							{
								const FLegacyGPUVert3PackedHalf& V0 = S.VertsHalfPacked[VertexID];
								Vertex = V0.Pos.ToVector(S.MeshOrigin, S.MeshExtension);
								V = &V0;
								SUV = V0.UV;
							}
							// UV
							for (int TexCoordIndex = 0; TexCoordIndex < NumTexCoords; TexCoordIndex++)
							{
								FLegacyMeshUVFloat FloatUV = SUV[TexCoordIndex];
								OutUV.UV[TexCoordIndex].X = FloatUV.U;
								OutUV.UV[TexCoordIndex].Y = FloatUV.V;
							}
						}
						else
						{
							// position
							const FLegacyMeshUVFloat* SUV;
							if (!S.bUsePackedPosition)
							{
								const FLegacyGPUVert3Float& V0 = S.VertsFloat[VertexID];
								V = &V0;
								Vertex = V0.Pos;
								SUV = V0.UV;
							}
							else
							{
								const FLegacyGPUVert3PackedFloat& V0 = S.VertsFloatPacked[VertexID];
								V = &V0;
								Vertex = V0.Pos.ToVector(S.MeshOrigin, S.MeshExtension);
								SUV = V0.UV;
							}
							// UV
							for (int TexCoordIndex = 0; TexCoordIndex < NumTexCoords; TexCoordIndex++)
							{
								FLegacyMeshUVFloat FloatUV = SUV[TexCoordIndex];
								OutUV.UV[TexCoordIndex].X = FloatUV.U;
								OutUV.UV[TexCoordIndex].Y = FloatUV.V;
							}
						}
					
						UnpackNormal(V->Normal);
						for (int32 i = 0; i < NUM_INFLUENCES_UE3; i++)
						{
							if (V->BoneWeight[i] == 0) continue;
							SkeletalMeshImportData::FRawBoneInfluence BoneInfluence;
							BoneInfluence.BoneIndex = SkeletalMeshChunk->Bones[V->BoneIndex[i]];
							BoneInfluence.Weight =  V->BoneWeight[i] * (1.f/255.f);
							BoneInfluence.VertexIndex = VertexID;
							InSkeletalMeshImportData.Influences.Add(BoneInfluence);
						}
					}
					else
					{
						// old UE3 version without a GPU skin
						// get vertex from chunk
						const FLegacyMeshUVFloat* SUV;
						if (VertexID < SkeletalMeshChunk->FirstVertex + SkeletalMeshChunk->NumRigidVerts)
						{
							// rigid vertex
							const FLegacyRigidVertex3& V0 = SkeletalMeshChunk->RigidVerts[VertexID - SkeletalMeshChunk->FirstVertex];
							// position and normal
							Vertex = V0.Pos;
							
							UnpackNormal(V0.Normal);
							// single influence
							SkeletalMeshImportData::FRawBoneInfluence BoneInfluence;
							BoneInfluence.BoneIndex = SkeletalMeshChunk->Bones[V0.BoneIndex];
							BoneInfluence.Weight =  1;
							BoneInfluence.VertexIndex = VertexID;
							InSkeletalMeshImportData.Influences.Add(BoneInfluence);
							SUV = V0.UV;
						}
						else
						{
							// soft vertex
							const FLegacySoftVertex3& V0 = SkeletalMeshChunk->SoftVerts[VertexID - SkeletalMeshChunk->FirstVertex - SkeletalMeshChunk->NumRigidVerts];
							// position and normal
							Vertex = V0.Pos;
							UnpackNormal(V0.Normal);
							// influences
							for (int32 i = 0; i < NUM_INFLUENCES_UE3; i++)
							{
								if (V0.BoneWeight[i] == 0) continue;
								SkeletalMeshImportData::FRawBoneInfluence BoneInfluence;
								BoneInfluence.BoneIndex = SkeletalMeshChunk->Bones[V0.BoneIndex[i]];
								BoneInfluence.Weight =  V0.BoneWeight[i] * (1.f/255.f);
								BoneInfluence.VertexIndex = VertexID;
								InSkeletalMeshImportData.Influences.Add(BoneInfluence);
							}
							SUV = V0.UV;
						}
						// UV
						for (int TexCoordIndex = 0; TexCoordIndex < NumTexCoords; TexCoordIndex++)
						{
							FLegacyMeshUVFloat FloatUV = SUV[TexCoordIndex];
							OutUV.UV[TexCoordIndex].X = FloatUV.U;
							OutUV.UV[TexCoordIndex].Y = FloatUV.V;
						}
					}
					InSkeletalMeshImportData.Points.Add(Vertex);
				}
				
				InSkeletalMeshImportData.MaxMaterialIndex = 0;
				TMap<int32,int32> LegacyMaterial2Material;
				for (size_t ElementID = 0; ElementID < LODModels[LodIndex].Sections.Num(); ElementID++)
				{
					
					FLegacySkeletalMeshSection3& Section = LODModels[LodIndex].Sections[ElementID];
					if (!LegacyMaterial2Material.Contains(Section.MaterialIndex))
					{
						SkeletalMeshImportData::FMaterial Material;
						Material.Material =  CastChecked<UMaterialInterface>(Materials[Section.MaterialIndex]->ExportToContent(),ECastCheckedType::NullAllowed);
						Material.MaterialImportName = *FString::Printf(TEXT("Mat_%d"),Section.MaterialIndex);
						int32 MaterialID = InSkeletalMeshImportData.Materials.Add(Material);
						LegacyMaterial2Material.Add(Section.MaterialIndex,MaterialID);
						InSkeletalMeshImportData.MaxMaterialIndex ++;
					}
				}

				for (size_t ElementID = 0; ElementID < LODModels[LodIndex].Sections.Num(); ElementID++)
				{
					
					FLegacySkeletalMeshSection3& Section = LODModels[LodIndex].Sections[ElementID];
					
					int32 MaterialID =LegacyMaterial2Material[Section.MaterialIndex];
					for (size_t FaceID = 0; FaceID < Section.NumTriangles; FaceID++)
					{
						int32 Index0 = 0;
						int32 Index1 = 0;
						int32 Index2 = 0;
						if (LODModels[LodIndex].IndexBuffer.Is32Bit())
						{
							Index0 = LODModels[LodIndex].IndexBuffer.Indices32[FaceID*3+0+Section.FirstIndex];
							Index1 = LODModels[LodIndex].IndexBuffer.Indices32[FaceID*3+1+Section.FirstIndex];
							Index2 = LODModels[LodIndex].IndexBuffer.Indices32[FaceID*3+2+Section.FirstIndex];
						}
						else
						{
							Index0 = LODModels[LodIndex].IndexBuffer.Indices16[FaceID*3+0+Section.FirstIndex];
							Index1 = LODModels[LodIndex].IndexBuffer.Indices16[FaceID*3+1+Section.FirstIndex];
							Index2 = LODModels[LodIndex].IndexBuffer.Indices16[FaceID*3+2+Section.FirstIndex];
						}
						
						SkeletalMeshImportData::FVertex Wedge;
						Wedge.VertexIndex = Index0;
						Wedge.MatIndex = MaterialID;
						Wedge.UVs[0] = UVs[Index0].UV[0];
						Wedge.UVs[1] = UVs[Index0].UV[1];
						Wedge.UVs[2] = UVs[Index0].UV[2];
						Wedge.UVs[3] = UVs[Index0].UV[3];
						if (InSkeletalMeshImportData.bHasVertexColors)
						{
							Wedge.Color = LODModels[LodIndex].VertexColor[Index0];
						}
						int32 WedgeID0 = InSkeletalMeshImportData.Wedges.Add(Wedge);
						
						Wedge.VertexIndex = Index1;
						Wedge.UVs[0] = UVs[Index1].UV[0];
						Wedge.UVs[1] = UVs[Index1].UV[1];
						Wedge.UVs[2] = UVs[Index1].UV[2];
						Wedge.UVs[3] = UVs[Index1].UV[3];
						if (InSkeletalMeshImportData.bHasVertexColors)
						{
							Wedge.Color = LODModels[LodIndex].VertexColor[Index1];
						}
						int32 WedgeID1 = InSkeletalMeshImportData.Wedges.Add(Wedge);
						
						Wedge.VertexIndex = Index2;
						Wedge.UVs[0] = UVs[Index2].UV[0];
						Wedge.UVs[1] = UVs[Index2].UV[1];
						Wedge.UVs[2] = UVs[Index2].UV[2];
						Wedge.UVs[3] = UVs[Index2].UV[3];
						if (InSkeletalMeshImportData.bHasVertexColors)
						{
							Wedge.Color = LODModels[LodIndex].VertexColor[Index2];
						}
						int32 WedgeID2 = InSkeletalMeshImportData.Wedges.Add(Wedge);

						//InSkeletalMeshImportData.MeshInfos.Add();

						SkeletalMeshImportData::FTriangle Triangle;
						Triangle.WedgeIndex[0] = WedgeID0;
						Triangle.WedgeIndex[1] = WedgeID1;
						Triangle.WedgeIndex[2] = WedgeID2;
						Triangle.MatIndex = MaterialID;
						
						Triangle.TangentX[0] = Normals[Index0].Tangent;
						Triangle.TangentY[0] = Normals[Index0].Binormal;
						Triangle.TangentZ[0] = Normals[Index0].Normal;

						
						Triangle.TangentX[1] = Normals[Index1].Tangent;
						Triangle.TangentY[1] = Normals[Index1].Binormal;
						Triangle.TangentZ[1] = Normals[Index1].Normal;

						
						Triangle.TangentX[2] = Normals[Index2].Tangent;
						Triangle.TangentY[2] = Normals[Index2].Binormal;
						Triangle.TangentZ[2] = Normals[Index2].Normal;
						InSkeletalMeshImportData.Faces.Add(Triangle);;
					}
				}
			}
			if (!CreateSkeletalMesh(SkeletalMesh, SkeletalMeshImportDataList, Bones, PackageName))
			{
				return nullptr;
			}
		}

		TArray<FSkeletalMaterial> OutSekeletalMaterials;
		for (ULegacyMaterialInterface*LegacyMaterial : Materials)
		{
			FSkeletalMaterial& SkeletalMaterial = OutSekeletalMaterials.AddDefaulted_GetRef();
			SkeletalMaterial.MaterialInterface = CastChecked<UMaterialInterface>(LegacyMaterial->ExportToContent(),ECastCheckedType::NullAllowed);
			SkeletalMaterial.MaterialSlotName = *FString::Printf(TEXT("Mat_%d"),OutSekeletalMaterials.Num()-1);
		}
		SkeletalMesh->SetMaterials(OutSekeletalMaterials);
		SkeletalMesh->PostEditChange();
		SkeletalMesh->Modify();
	}
	PresentObject = SkeletalMesh;
	return SkeletalMesh;
}

bool ULegacySkeletalMesh3::FindOrAddSkeleton(USkeletalMesh* SkeletalMesh, const FString& FullName)
{
	URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
	
	for (USkeleton* Skeleton : RedUELegacySubsystem->Skeletons)
	{
		const FReferenceSkeleton& SkeletonRefSkel = Skeleton->GetReferenceSkeleton();
		const FReferenceSkeleton& MeshRefSkel = SkeletalMesh->GetRefSkeleton();

		if (SkeletonRefSkel.GetNum() != MeshRefSkel.GetNum())
		{
			continue;
		}
		if (SkeletonRefSkel.GetBoneName(0) != MeshRefSkel.GetBoneName(0))
		{
			continue;
		}
		TMap<FName,FName> Bones;
		for (int32 i=1;i< SkeletonRefSkel.GetNum();i++)
		{
			Bones.Add(SkeletonRefSkel.GetBoneName(i), SkeletonRefSkel.GetBoneName(SkeletonRefSkel.GetParentIndex(i)));
		}
		bool bResult = true;
		for (int32 i = 1; i < MeshRefSkel.GetNum(); i++)
		{
			if (!Bones.Contains(MeshRefSkel.GetBoneName(i))||Bones[MeshRefSkel.GetBoneName(i)] != MeshRefSkel.GetBoneName(MeshRefSkel.GetParentIndex(i)))
			{
				bResult = false;
			}
		}
		if (bResult)
		{
			SkeletalMesh->SetSkeleton(Skeleton);
			return true; 
		}
	}
	
	const FString NewObjectPath = FullName + TEXT(".") + FPaths::GetBaseFilename(FullName);
	USkeleton* Skeleton = LoadObject<USkeleton>(nullptr, *NewObjectPath);
	if (Skeleton)
	{
		RedUELegacySubsystem->Skeletons.Add(Skeleton);
		return Skeleton != nullptr;
	}
	UPackage* AssetPackage = CreatePackage(*FullName);
	Skeleton = NewObject<USkeleton>(AssetPackage, *FPaths::GetBaseFilename(FullName),RF_Public | RF_Standalone);
	Skeleton->MergeAllBonesToBoneTree(SkeletalMesh);
	Skeleton->SetPreviewMesh(SkeletalMesh);
	SkeletalMesh->SetSkeleton(Skeleton);
	FAssetRegistryModule::AssetCreated(Skeleton);
	RedUELegacySubsystem->Skeletons.Add(Skeleton);
	return Skeleton != nullptr;
}

bool ULegacySkeletalMesh3::CreateSkeletalMesh(USkeletalMesh* SkeletalMesh, TArray<FSkeletalMeshImportData>& LODIndexToSkeletalMeshImportData, const TArray<SkeletalMeshImportData::FBone>& InSkeletonBones, const FString& SkeletalMeshName)
{
	if (LODIndexToSkeletalMeshImportData.Num() == 0 || InSkeletonBones.Num() == 0)
	{
		return false;
	}

	// A SkeletalMesh could be retrieved for re-use and updated for animations
	// For now, create a new USkeletalMesh
	// Note: Remember to initialize UsedMorphTargetNames with existing morph targets, whenever the SkeletalMesh is reused
	

	// Process reference skeleton from import data
	int32 LocalSkeletalDepth = 0;
	FSkeletalMeshImportData DummyData;
	DummyData.RefBonesBinary = InSkeletonBones;

	if (!SkeletalMeshImportUtils::ProcessImportMeshSkeleton(SkeletalMesh->GetSkeleton(), SkeletalMesh->GetRefSkeleton(), LocalSkeletalDepth, DummyData))
	{
		return false;
	}

	if (SkeletalMesh->GetRefSkeleton().GetRawBoneNum() == 0)
	{
		SkeletalMesh->MarkAsGarbage();
	}

	// This prevents PostEditChange calls when it is alive, also ensuring it is called once when we return from this function.
	// This is required because we must ensure the morphtargets are in the SkeletalMesh before the first call to PostEditChange(),
	// or else they will be effectively discarded
	FScopedSkeletalMeshPostEditChange ScopedPostEditChange(SkeletalMesh);
	SkeletalMesh->PreEditChange(nullptr);

	// Create initial bounding box based on expanded version of reference pose for meshes without physics assets
	const FSkeletalMeshImportData& LowestLOD = LODIndexToSkeletalMeshImportData[0];
	FBox3f BoundingBox(LowestLOD.Points.GetData(), LowestLOD.Points.Num());
	FBox3f Temp = BoundingBox;
	FVector3f MidMesh = 0.5f * (Temp.Min + Temp.Max);
	BoundingBox.Min = Temp.Min + 1.0f * (Temp.Min - MidMesh);
	BoundingBox.Max = Temp.Max + 1.0f * (Temp.Max - MidMesh);
	BoundingBox.Min[2] = Temp.Min[2] + 0.1f * (Temp.Min[2] - MidMesh[2]);
	const FVector3f BoundingBoxSize = BoundingBox.GetSize();
	if (LowestLOD.Points.Num() > 2 && BoundingBoxSize.X < THRESH_POINTS_ARE_SAME && BoundingBoxSize.Y < THRESH_POINTS_ARE_SAME && BoundingBoxSize.Z < THRESH_POINTS_ARE_SAME)
	{
		return false;
	}

#if WITH_EDITOR
	IMeshUtilities& MeshUtilities = FModuleManager::Get().LoadModuleChecked<IMeshUtilities>("MeshUtilities");
#endif // WITH_EDITOR

	FSkeletalMeshModel* ImportedResource = SkeletalMesh->GetImportedModel();
	ImportedResource->LODModels.Empty();
	SkeletalMesh->ResetLODInfo();
	bool bHasVertexColors = false;


	for (int32 LODIndex = 0; LODIndex < LODIndexToSkeletalMeshImportData.Num(); ++LODIndex)
	{
		FSkeletalMeshImportData& LODImportData = LODIndexToSkeletalMeshImportData[LODIndex];
		ImportedResource->LODModels.Add(new FSkeletalMeshLODModel());
		FSkeletalMeshLODModel& LODModel = ImportedResource->LODModels.Last();

		// Process bones influence (normalization and optimization) (optional)
		SkeletalMeshImportUtils::ProcessImportMeshInfluences(LODImportData, SkeletalMesh->GetPathName());

		FSkeletalMeshLODInfo& NewLODInfo = SkeletalMesh->AddLODInfo();
		NewLODInfo.ReductionSettings.NumOfTrianglesPercentage = 1.0f;
		NewLODInfo.ReductionSettings.NumOfVertPercentage = 1.0f;
		NewLODInfo.ReductionSettings.MaxDeviationPercentage = 0.0f;
		NewLODInfo.LODHysteresis = 0.02f;

		bHasVertexColors |= LODImportData.bHasVertexColors;

		LODModel.NumTexCoords = FMath::Max<uint32>(1, LODImportData.NumTexCoords);

		// Data needed by BuildSkeletalMesh
		LODImportData.PointToRawMap.AddUninitialized(LODImportData.Points.Num());
		for (int32 PointIndex = 0; PointIndex < LODImportData.Points.Num(); ++PointIndex)
		{
			LODImportData.PointToRawMap[PointIndex] = PointIndex;
		}

		TArray<FVector3f> LODPoints;
		TArray<SkeletalMeshImportData::FMeshWedge> LODWedges;
		TArray<SkeletalMeshImportData::FMeshFace> LODFaces;
		TArray<SkeletalMeshImportData::FVertInfluence> LODInfluences;
		TArray<int32> LODPointToRawMap;
		LODImportData.CopyLODImportData(LODPoints, LODWedges, LODFaces, LODInfluences, LODPointToRawMap);

#if WITH_EDITOR
		IMeshUtilities::MeshBuildOptions BuildOptions;
		BuildOptions.TargetPlatform = GetTargetPlatformManagerRef().GetRunningTargetPlatform();
		// #ueent_todo: Normals and tangents shouldn't need to be recomputed when they are retrieved from USD
		BuildOptions.bComputeNormals = !LODImportData.bHasNormals;
		BuildOptions.bComputeTangents = !LODImportData.bHasTangents;
		BuildOptions.bUseMikkTSpace = true;
		TArray<FText> WarningMessages;
		TArray<FName> WarningNames;

		bool bBuildSuccess = MeshUtilities.BuildSkeletalMesh(LODModel, SkeletalMesh->GetPathName(), SkeletalMesh->GetRefSkeleton(), LODInfluences, LODWedges, LODFaces, LODPoints, LODPointToRawMap, BuildOptions, &WarningMessages, &WarningNames);

		for (int32 WarningIndex = 0; WarningIndex < FMath::Max(WarningMessages.Num(), WarningNames.Num()); ++WarningIndex)
		{
			const FText& Text = WarningMessages.IsValidIndex(WarningIndex) ? WarningMessages[WarningIndex] : FText::GetEmpty();
			const FName& Name = WarningNames.IsValidIndex(WarningIndex) ? WarningNames[WarningIndex] : NAME_None;

			if (bBuildSuccess)
			{
				UE_LOG(LogRedUELegacy, Warning, TEXT("Warning when trying to build skeletal mesh from USD: '%s': '%s'"), *Name.ToString(), *Text.ToString());
			}
			else
			{
				UE_LOG(LogRedUELegacy, Error, TEXT("Error when trying to build skeletal mesh from USD: '%s': '%s'"), *Name.ToString(), *Text.ToString());
			}
		}

		if (!bBuildSuccess)
		{
			SkeletalMesh->MarkAsGarbage();
			return false;
		}

		// This is important because it will fill in the LODModel's RawSkeletalMeshBulkDataID,
		// which is the part of the skeletal mesh's DDC key that is affected by the actual mesh data
		SkeletalMesh->SaveLODImportedData(LODIndex, LODImportData);
#endif // WITH_EDITOR
	}

	SkeletalMesh->SetImportedBounds(FBoxSphereBounds((FBox)BoundingBox));
	SkeletalMesh->SetHasVertexColors(bHasVertexColors);
	SkeletalMesh->SetVertexColorGuid(SkeletalMesh->GetHasVertexColors() ? FGuid::NewGuid() : FGuid());
	SkeletalMesh->CalculateInvRefMatrices();

	// Generate a Skeleton and associate it to the SkeletalMesh
	FString SkeletonName = SkeletalMeshName + "_Skeleton";


	if (!FindOrAddSkeleton(SkeletalMesh, SkeletonName))
	{
		return false;
	}
//	CreateMorphTargets(SkeletalMeshImport, LODIndexToSkeletalMeshImportData, SkeletalMesh,ParentPackage,Flags,SkeletalMeshName);
	return true;
}

