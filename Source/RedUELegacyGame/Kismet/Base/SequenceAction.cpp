#include "SequenceAction.h"
#include "LegacyKismet.h"

void USequenceAction::Construct()
{
	if (ALegacyKismet* LegacyKismet = GetTypedOuter<ALegacyKismet>())
	{
		for (auto&[Key,Value]:InitializeDelegates)
		{
			if (FMulticastDelegateProperty* DelegateProperty =  CastField<FMulticastDelegateProperty>(GetClass()->FindPropertyByName(Key)))
			{
				if (UFunction* NewFunction = LegacyKismet->GetClass()->FindFunctionByName(Value))
				{
					FScriptDelegate Delegate;
					Delegate.BindUFunction(LegacyKismet, Value);
					DelegateProperty->AddDelegate(Delegate,this);
				}
			}
		}
	}
}

void USequenceAction::BeginPlay()
{
	
}

void USequenceAction::Tick(float DeltaTime)
{
}
