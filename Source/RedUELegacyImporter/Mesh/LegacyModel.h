#pragma once

#include "CoreMinimal.h"
#include "Core/LegacyObject.h"
#include "Material/LegacyMaterialInstance.h"
#include "LegacyModel.generated.h"



struct FLightingChannelContainer
{
	union
	{
		struct
		{
			/** Whether the lighting channel has been initialized. Used to determine whether UPrimitveComponent::Attach should set defaults. */
			int bInitialized : 1;

			// User settable channels that are auto set and also default to true for lights.
			int BSP : 1;
			int Static : 1;
			int Dynamic : 1;
			// User set channels.
			int CompositeDynamic : 1;
			int Skybox : 1;
			int Unnamed_1 : 1;
			int Unnamed_2 : 1;
			int Unnamed_3 : 1;
			int Unnamed_4 : 1;
			int Unnamed_5 : 1;
			int Unnamed_6 : 1;
			int Cinematic_1 : 1;
			int Cinematic_2 : 1;
			int Cinematic_3 : 1;
			int Cinematic_4 : 1;
			int Cinematic_5 : 1;
			int Cinematic_6 : 1;
			int Cinematic_7 : 1;
			int Cinematic_8 : 1;
			int Cinematic_9 : 1;
			int Cinematic_10 : 1;
			int Gameplay_1 : 1;
			int Gameplay_2 : 1;
			int Gameplay_3 : 1;
			int Gameplay_4 : 1;
			int Crowd : 1;
		};
		int Bitfield;
	};
};

class ULegacyMaterialInstance;
class ULegacyActor;

struct FLegacyBspSurf
{
public:

	ULegacyMaterialInstance*	Material = nullptr;		// 4 Material.
	int32						PolyFlags = 0;		// 4 Polygon flags.
	int32						pBase = 0;			// 4 Polygon & texture base point index (where U,V==0,0).
	int32						vNormal = 0;		// 4 Index to polygon normal.
	int32						vTextureU = 0;		// 4 Texture U-vector index.
	int32						vTextureV = 0;		// 4 Texture V-vector index.
	int32						iBrushPoly = 0;		// 4 Editor brush polygon index.
	ULegacyActor*				Actor = nullptr;			// 4 Brush actor owning this Bsp surface.
	FPlane4f					Plane;			// 16 The plane this surface lies on.
	float						ShadowMapScale = 0.f;	// 4 The number of units/lightmap texel on this surface.
	FLightingChannelContainer	LightingChannels;	// 4 Lighting channels of affecting lights.

	int32						iLightmassIndex = 0;// 4 Index to the lightmass settings

	int32						bHiddenEdTemporary = 0;	// 4 Marks whether this surface is temporarily hidden in the editor or not. Not serialized.
	int32						bHiddenEdLevel = 0;		// 4 Marks whether this surface is hidden by the level browser or not. Not serialized.

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyBspSurf& Surf);
};


/**
 * 
 */
UCLASS(meta = (LegacyPackage = Engine))
class REDUELEGACYIMPORTER_API ULegacyModel : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual void			LegacySerialize	(FRedUELegacyArchive& Ar) override;
			void			Fill			(ABrush* Brush);

	FBoxSphereBounds3f		Bounds;
	TArray<FBspNode>		Nodes;
	TArray<FLegacyBspSurf>	Surfs;
	TArray<FVector3f>		Points;
	TArray<FVector3f>		Vectors;
	TArray<FVert>			Verts;
};
