#include "LegacyRvComponents.h"

ULegacyRvActorComponent* ULegacyRvComponents::GetRvComponent(ERvComponentType Type) const
{
	return mComponents[static_cast<int32>(Type)];
}

void ULegacyRvComponents::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
}
