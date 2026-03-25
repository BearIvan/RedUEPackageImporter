#include "Mesh/LegacyModel.h"

#include "BSPOps.h"
#include "BSPUtils.h"
#include "Editor.h"
#include "Core/RedUELegacyArchive.h"
#include "Editor/EditorEngine.h"
#include "World/Actors/LegacyActor.h"

FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FLegacyBspSurf& Surf)
{
	Ar << Surf.Material;
	Ar << Surf.PolyFlags;
	Ar << Surf.pBase << Surf.vNormal;
	Ar << Surf.vTextureU << Surf.vTextureV;
	Ar << Surf.iBrushPoly;
	Ar << Surf.Actor;
	Ar << Surf.Plane;
	Ar << Surf.ShadowMapScale;
	if (Ar.Game != ERedUELegacyGame::Bioshock3)
	{
		int LightingChannels = Surf.LightingChannels.Bitfield;
		Ar << LightingChannels;
		Surf.LightingChannels.Bitfield = LightingChannels;
	}
	if (Ar.LegacyVer < 600 )
	{
		Surf.iLightmassIndex = 0;
	}
	else
	{
		Ar << Surf.iLightmassIndex;
	}
	return Ar;
}

void ULegacyModel::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	if (Ar.Game == ERedUELegacyGame::Bioshock3)
	{
		int32 bHasCachedBounds;
		Ar<<bHasCachedBounds;
	}
	Ar<<Bounds;
	Ar.LegacyBulkSerialize(Vectors);
	Ar.LegacyBulkSerialize(Points);
	Ar.LegacyBulkSerialize(Nodes);
	{
		UObject*Owner;
		Ar<<Owner;
	}
	Ar<<Surfs;
	Ar.LegacyBulkSerialize(Verts);
}

void ULegacyModel::Fill(ABrush* BrushActor)
{
	BrushActor->Modify();
	BrushActor->PreEditChange(nullptr);
	FTransform ActorTransform = BrushActor->GetActorTransform();
	UBrushComponent* BrushComponent = BrushActor->GetBrushComponent();
	BrushComponent->Modify();
	BrushComponent->PreEditChange(nullptr);
	BrushComponent->Brush = NewObject<UModel>(BrushActor);
	BrushComponent->Brush->Initialize(BrushActor);
	BrushComponent->Brush->EmptyModel(1,1);
	BrushComponent->Brush->Bounds.BoxExtent = FVector(Bounds.BoxExtent);
	BrushComponent->Brush->Bounds.Origin = FVector(Bounds.Origin);
	BrushComponent->Brush->Bounds.SphereRadius = Bounds.SphereRadius;
	BrushComponent->Brush->Points = Points;
	BrushComponent->Brush->Verts = Verts;
	BrushComponent->Brush->Vectors = Vectors;
	BrushComponent->Brush->Nodes =Nodes;
	for (const FLegacyBspSurf& Surf :Surfs)
	{
		FBspSurf&OutSurf = BrushComponent->Brush->Surfs.AddDefaulted_GetRef();
		OutSurf.Actor = BrushActor;
		OutSurf.Material = Surf.Material? Cast<UMaterialInterface>(Surf.Material->ExportToContent()) : nullptr;
		OutSurf.PolyFlags = Surf.PolyFlags;
		OutSurf.pBase = Surf.pBase;
		OutSurf.vNormal = Surf.vNormal;
		OutSurf.vTextureU = Surf.vTextureU;
		OutSurf.vTextureV = Surf.vTextureV;
		OutSurf.iBrushPoly = Surf.iBrushPoly;
		OutSurf.Plane = Surf.Plane;
		OutSurf.LightMapScale = Surf.ShadowMapScale;
		OutSurf.iLightmassIndex = Surf.iLightmassIndex;
		if (Surf.iLightmassIndex >= BrushComponent->Brush->LightmassSettings.Num())
		{
			BrushComponent->Brush->LightmassSettings.AddDefaulted( Surf.iLightmassIndex - BrushComponent->Brush->LightmassSettings.Num() + 1);
		}
	}
	{
		TSet<int32> AddPolys;
		TFunction<void(int32)> MakeEdPolys = [Model = BrushComponent->Brush,&MakeEdPolys,&AddPolys](int32 iNode)
		{
			if (AddPolys.Contains(iNode))
			{
				return;
			}
			AddPolys.Add(iNode);
			
			FBspNode* Node = &Model->Nodes[iNode];

			FPoly Temp;
			if( FBSPUtils::bspNodeToFPoly(Model,iNode,&Temp) >= 3 )
			{
				Model->Polys->Element.Add(Temp);
			}

			if( Node->iFront!=INDEX_NONE )
			{
				MakeEdPolys( Node->iFront );
			}
			if( Node->iBack !=INDEX_NONE ) 
			{
				MakeEdPolys( Node->iBack );
			}
			if( Node->iPlane!=INDEX_NONE )
			{
				MakeEdPolys(  Node->iPlane );
			}
		};
		for (int32  iNode = 0 ; iNode < Nodes.Num() ; iNode++)
		{
			MakeEdPolys(iNode);
		}
		FBSPOps::bspRefresh( BrushComponent->Brush, 1 );
		FBSPOps::bspValidateBrush( BrushComponent->Brush, 0, 1 );

	}
	if (BrushActor->IsStaticBrush()	)
	{
		ABrush::SetNeedRebuild(BrushActor->GetLevel());
	}
	else
	{
		FBSPOps::csgPrepMovingBrush(BrushActor);
		BrushComponent->BuildSimpleBrushCollision();
	}
	
	BrushComponent->PostEditChange();
	BrushActor->PostEditChange();
	BrushActor->PostEditMove(true);
	BrushActor->SetActorTransform(ActorTransform);
}
