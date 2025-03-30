// Tyran


#include "World/Actors/LegacyActor.h"

#include "Core/RedUELegacyArchive.h"

void ULegacyActor::Spawn_Implementation()
{
}

bool FLegacyRotator::Serialize(FArchive& Ar)
{
	Ar << Pitch << Yaw << Roll;
	return true;
}

void ULegacyActor::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	if (Ar.Game == ERedUELegacyGame::Bioshock3)
	{
		Rotation.Yaw = XPrivateLocalRotation.Yaw*(180.f / 32768.f);
		Rotation.Roll = XPrivateLocalRotation.Roll*(180.f / 32768.f);
		Rotation.Pitch = XPrivateLocalRotation.Pitch*(180.f / 32768.f);
		Translation = XPrivateLocalLocation;
		Scale3D = DrawScale3D;
	}
}
