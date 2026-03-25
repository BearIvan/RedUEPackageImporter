#include "LegacyKismetGeneratedClass.h"

void ULegacyKismetGeneratedClass::PurgeClass(bool bRecompilingOnLoad)
{
	Super::PurgeClass(bRecompilingOnLoad);
	StartupActions.Empty();
}
