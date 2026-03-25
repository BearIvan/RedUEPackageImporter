#include "MaterialInstanceHybridFactory.h"
#include "Materials/Hybrid/MaterialInstanceHybrid.h"

UMaterialInstanceHybridFactory::UMaterialInstanceHybridFactory()
{
	SupportedClass = UMaterialInstanceHybrid::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UMaterialInstanceHybridFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UMaterialInstanceHybrid::StaticClass()));

	UMaterialInstanceHybrid* NewInstance = NewObject<UMaterialInstanceHybrid>(InParent, Class, Name, Flags | RF_Transactional);
	check(NewInstance);

	return NewInstance;
}

