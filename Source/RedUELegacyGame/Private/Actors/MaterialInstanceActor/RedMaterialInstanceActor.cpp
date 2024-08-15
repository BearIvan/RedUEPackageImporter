#include "Actors/MaterialInstanceActor/RedMaterialInstanceActor.h"


ARedMaterialInstanceActor::ARedMaterialInstanceActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARedMaterialInstanceActor::SetScalarParameter(FName ParameterName, float Scalar)
{
}

void ARedMaterialInstanceActor::SetVectorParameter(FName ParameterName, const FVector4f& Vector)
{
}
