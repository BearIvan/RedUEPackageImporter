#include "SequenceAction.h"
#include "LegacyKismet.h"
#include "Kismet2/BlueprintEditorUtils.h"

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

void USequenceAction::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
	if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(PropertyChangedEvent.Property))
	{
		if (FSequenceActionPropertyArrayReference* PropertyReference = PropertiesArrayReference.Find(ArrayProperty->GetFName()))
		{
			if (PropertiesReference.Contains(ArrayProperty->GetFName()))
			{
				PropertiesArrayReference.Remove(ArrayProperty->GetFName());
				return;
			}
			TArray<int32> ArrayIndexes;
			PropertyReference->ArrayElementReference.GetKeys(ArrayIndexes);
			

			FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(this));
			int32 ArrayNum = ArrayHelper.Num();
			
			for (int32 Index :ArrayIndexes)
			{
				if (Index >= ArrayNum)
				{
					PropertyReference->ArrayElementReference.Remove(Index);
				}
			}
			if (PropertyReference->ArrayElementReference.Num() == 0)
			{
				PropertiesArrayReference.Remove(ArrayProperty->GetFName());
			}
			if (UBlueprint* Blueprint = GetTypedOuter<UBlueprint>())
			{
				FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
			}
		}
	}
	else
	{
		if (FSequenceActionPropertyArrayReference* PropertyReference = PropertiesArrayReference.Find(ArrayProperty->GetFName()))
		{
			PropertiesArrayReference.Remove(ArrayProperty->GetFName());
		}
	}

	
}
