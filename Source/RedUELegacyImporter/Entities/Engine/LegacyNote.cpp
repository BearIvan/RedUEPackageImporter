#include "LegacyNote.h"

UClass* ULegacyNote::GetActorClass_Implementation()
{
	return ANote::StaticClass();
}

void ULegacyNote::FillActor_Implementation(AActor* InActor)
{
	if (ANote* Note = CastChecked<ANote>(InActor))
	{
		Note->Text = Text;
	}
}
