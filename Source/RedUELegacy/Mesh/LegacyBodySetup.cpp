#include "Mesh/LegacyBodySetup.h"

#include "PhysicsEngine/BodySetup.h"

bool FLegacyKConvexElem::Serialize(FArchive& Ar)
{
	if (Ar.IsLoading() || Ar.IsSaving())
	{
		Ar << VertexData;
		Ar << PermutedVertexData;
		Ar << FaceTriData;
		Ar << EdgeDirections;
		Ar << FaceNormalDirections;
		Ar << FacePlaneData;
		Ar << ElemBox;
	}
	return true;
	
}

void ULegacyRB_BodySetup::PushTo(UBodySetup* ToBodySetup)
{
	if (ensure(ToBodySetup))
	{
		for (const FLegacyKBoxElem& BoxElem :AggGeom.BoxElems)
		{
			FTransform Transform = FTransform(BoxElem.TM);
			
			FKBoxElem NewBoxElem(BoxElem.X, BoxElem.Y, BoxElem.Z);
			NewBoxElem.Center = Transform.GetLocation();
			NewBoxElem.Rotation = Transform.GetRotation().Rotator();
			ToBodySetup->AggGeom.AddElement(NewBoxElem);
		}
		
		for (const FLegacyKSphereElem& SphereElem :AggGeom.SphereElems)
		{
			FTransform Transform = FTransform(SphereElem.TM);
			
			FKSphereElem NewSphereElem(SphereElem.Radius);
			NewSphereElem.Center = Transform.GetLocation();
			ToBodySetup->AggGeom.AddElement(NewSphereElem);
		}
		
		for (const FLegacyKSphylElem& SphylElems :AggGeom.SphylElems)
		{
			FTransform Transform = FTransform(SphylElems.TM);
			
			FKSphylElem NewSphylElem(SphylElems.Radius,SphylElems.Length);
			NewSphylElem.Center = Transform.GetLocation();
			NewSphylElem.Rotation = Transform.GetRotation().Rotator();
			ToBodySetup->AggGeom.AddElement(NewSphylElem);
		}
		
		for (const FLegacyKConvexElem& ConvexElem :AggGeom.ConvexElems)
		{
			FKConvexElem NewConvexElem;
			NewConvexElem.ElemBox = FBox(ConvexElem.ElemBox);
			for (int32  i = 0;i<ConvexElem.FaceTriData.Num()/3;i++)
			{
				NewConvexElem.IndexData.Add(ConvexElem.FaceTriData[i*3 + 0]);
				NewConvexElem.IndexData.Add(ConvexElem.FaceTriData[i*3 + 2]);
				NewConvexElem.IndexData.Add(ConvexElem.FaceTriData[i*3 + 1]);
			}
			for (const FVector3f& Vertex : ConvexElem.VertexData)
			{
				NewConvexElem.VertexData.Add(FVector(Vertex));
			}
			ToBodySetup->AggGeom.AddElement(NewConvexElem);
		}
	}
}
