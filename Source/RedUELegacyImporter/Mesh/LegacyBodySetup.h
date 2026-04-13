#pragma once

#include "Core/LegacyObject.h"
#include "LegacyBodySetup.generated.h"


USTRUCT()
struct FLegacyKSphereElem
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyMatrix TM;
	
	UPROPERTY(meta = (LegacyRead))
	float Radius;
};


USTRUCT()
struct FLegacyKBoxElem
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyMatrix TM;
	
	UPROPERTY(meta = (LegacyRead))
	float X;
	
	UPROPERTY(meta = (LegacyRead))
	float Y;
	
	UPROPERTY(meta = (LegacyRead))
	float Z;
};

USTRUCT()
struct FLegacyKSphylElem
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (LegacyRead))
	FLegacyMatrix TM;
	
	UPROPERTY(meta = (LegacyRead))
	float Radius;
	
	UPROPERTY(meta = (LegacyRead))
	float Length;
};

USTRUCT()
struct FLegacyKConvexElem
{
	GENERATED_BODY()
	bool Serialize(FArchive& Ar);
	
	UPROPERTY(meta = (LegacyRead))
	TArray<FVector3f> VertexData;
	UPROPERTY(meta = (LegacyRead))
	TArray<FPlane4f> PermutedVertexData;
	UPROPERTY(meta = (LegacyRead))
	TArray<int32> FaceTriData;
	UPROPERTY(meta = (LegacyRead))
	TArray<FVector3f> EdgeDirections;
	UPROPERTY(meta = (LegacyRead))
	TArray<FVector3f> FaceNormalDirections;
	UPROPERTY(meta = (LegacyRead))
	TArray<FPlane4f> FacePlaneData;
	UPROPERTY(meta = (LegacyRead))
    FBox3f ElemBox;
};

inline FArchive& operator<<(FArchive& Ar, FLegacyKConvexElem& R)
{
	R.Serialize(Ar);
	return Ar;
}


USTRUCT()
struct FKLegacyAggregateGeom
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (LegacyRead))
	TArray<FLegacyKSphereElem> SphereElems;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<FLegacyKBoxElem> BoxElems;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<FLegacyKSphylElem> SphylElems;
	
	UPROPERTY(meta = (LegacyRead))
	TArray<FLegacyKConvexElem> ConvexElems;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyKMeshProps : public ULegacyObject
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (LegacyRead))
	FKLegacyAggregateGeom AggGeom;
};



/**
 * 
 */
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyRB_BodySetup : public ULegacyKMeshProps
{
	GENERATED_BODY()
public:
	void PushTo(UBodySetup* ToBodySetup);
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyRB_ConstraintInstance : public ULegacyObject
{
	GENERATED_BODY()
public:
	void PushTo(UBodySetup* ToBodySetup);
};
