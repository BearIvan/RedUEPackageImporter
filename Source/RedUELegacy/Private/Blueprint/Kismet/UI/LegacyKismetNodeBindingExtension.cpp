#include "LegacyKismetNodeBindingExtension.h"

#include "BlueprintEditorModule.h"
#include "Editor.h"
#include "IPropertyAccessEditor.h"
#include "K2Node_FunctionEntry.h"
#include "Blueprint/Kismet/K2Node_SequenceAction.h"
#include "Blueprint/Kismet/LegacyKismetBlueprint.h"
#include "Engine/MemberReference.h"
#include "Kismet/Base/SequenceAction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "UObject/UnrealTypePrivate.h"

bool FLegacyKismetNodeBindingExtension::IsPropertyExtendable(const UClass* InObjectClass, const IPropertyHandle& PropertyHandle) const
{
	TArray<UObject*> Objects;
	PropertyHandle.GetOuterObjects(Objects);

	FProperty* Property = PropertyHandle.GetProperty();
	if(Property)
	{
		USequenceAction* SequenceAction  = Cast<USequenceAction>(Objects[0]);
		if (SequenceAction != nullptr)
		{
			if (FArrayProperty* ArrayProperty =  CastField<FArrayProperty>(Property->GetOwnerProperty()))
			{
				return ArrayProperty->HasMetaData(TEXT("KismetExternalVariable"));
			}
			return Property->HasMetaData(TEXT("KismetExternalVariable"));
		}
	}
	
	return false;
}

void FLegacyKismetNodeBindingExtension::ExtendWidgetRow(FDetailWidgetRow& InWidgetRow, const IDetailLayoutBuilder& InDetailBuilder, const UClass* InObjectClass, TSharedPtr<IPropertyHandle> InPropertyHandle)
{

	TArray<UObject*> OuterObjects;
	InPropertyHandle->GetOuterObjects(OuterObjects);

	FProperty* NodeProperty = InPropertyHandle->GetProperty();
	TSharedPtr<IPropertyHandle> ParentHandle = InPropertyHandle->GetParentHandle();

	FArrayProperty* OwnerArrayProperty = CastField<FArrayProperty>(NodeProperty->GetOwnerProperty());
	
	if (OwnerArrayProperty == NodeProperty)
	{
		OwnerArrayProperty = nullptr;
	}
	
	FProperty* OwnerProperty = NodeProperty;
	if (OwnerArrayProperty)
	{
		OwnerProperty = OwnerArrayProperty;
	}
	
	if(IModularFeatures::Get().IsModularFeatureAvailable("PropertyAccessEditor"))
	{
		USequenceAction* SequenceAction  = CastChecked<USequenceAction>(OuterObjects[0]);
		UK2Node_SequenceAction* SequenceActionNode  = CastChecked<UK2Node_SequenceAction>(SequenceAction->GetOuter());
		UBlueprint* Blueprint = SequenceActionNode->GetBlueprint();
		

		if(Blueprint == nullptr)
		{
			return;
		}
	
		auto OnGenerateBindingName = []() -> FString
		{
			return TEXT("NewFunction");
		};
		
		auto OnCanBindProperty = [NodeProperty,OwnerArrayProperty](FProperty* InProperty)
		{
			
			IPropertyAccessEditor& PropertyAccessEditor = IModularFeatures::Get().GetModularFeature<IPropertyAccessEditor>("PropertyAccessEditor");
			if (OwnerArrayProperty)
			{
				if (PropertyAccessEditor.GetPropertyCompatibility(InProperty, OwnerArrayProperty) != EPropertyAccessCompatibility::Incompatible)
				{
					return true;
				}
			}
			
			if (NodeProperty)
			{
				return PropertyAccessEditor.GetPropertyCompatibility(InProperty, NodeProperty) != EPropertyAccessCompatibility::Incompatible;
			}
			
			return false;
		};

		auto OnGoToBinding = [InPropertyHandle, Blueprint](FName InPropertyName)
		{
			return false;
		};
		
		auto OnCanGotoBinding = [InPropertyHandle](FName InPropertyName)
		{
			return false;
		};
		
		auto OnCanBindFunction = [](UFunction* InFunction)
		{
			return false;
		};

		auto OnAddBinding = [InPropertyHandle, Blueprint,OwnerProperty,OwnerArrayProperty,SequenceAction](FName InPropertyName, const TArray<FBindingChainElement>& InBindingChain)
		{
			void* Data = nullptr;
			const FPropertyAccess::Result Result = InPropertyHandle->GetValueData(Data);
			if(Result == FPropertyAccess::Success)
			{
				SequenceAction->PreEditChange(nullptr);
				

				FProperty* InProperty = InBindingChain[0].Field.Get<FProperty>();
				UClass* OwnerClass = InProperty ? InProperty->GetOwnerClass() : nullptr;
				bool bSelfContext = false;
				if(OwnerClass != nullptr)
				{
					bSelfContext = (Blueprint->GeneratedClass != nullptr && Blueprint->GeneratedClass->IsChildOf(OwnerClass)) ||
									(Blueprint->SkeletonGeneratedClass != nullptr && Blueprint->SkeletonGeneratedClass->IsChildOf(OwnerClass));
				}
				
				if (OwnerArrayProperty)
				{
					FSequenceActionPropertyArrayReference& MemberReference =  SequenceAction->PropertiesArrayReference.FindOrAdd(OwnerProperty->GetFName());
					MemberReference.ArrayElementReference.FindOrAdd(InPropertyHandle->GetArrayIndex()).SetFromField<FProperty>(InProperty, bSelfContext);
				}
				else
				{
		
					FMemberReference& MemberReference =  SequenceAction->PropertiesReference.FindOrAdd(OwnerProperty->GetFName());
					MemberReference.SetFromField<FProperty>(InProperty, bSelfContext);
				}
				
				SequenceAction->PostEditChange();
				SequenceAction->Modify();
				FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
			}
		};

		auto OnRemoveBinding = [SequenceAction,OwnerProperty,InPropertyHandle,OwnerArrayProperty,Blueprint](FName InPropertyName)
		{
			SequenceAction->PreEditChange(nullptr);
			if (OwnerArrayProperty)
			{
				if (FSequenceActionPropertyArrayReference* MemberReference =  SequenceAction->PropertiesArrayReference.Find(OwnerProperty->GetFName()))
				{
					MemberReference->ArrayElementReference.Remove(InPropertyHandle->GetArrayIndex());
					if (MemberReference->ArrayElementReference.Num() == 0)
					{
						SequenceAction->PropertiesArrayReference.Remove(OwnerProperty->GetFName());
					}
				}

			}
			else
			{
				SequenceAction->PropertiesReference.Remove(OwnerProperty->GetFName());
			}
			SequenceAction->PostEditChange();
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
		};

		auto CanRemoveBinding = [SequenceAction,OwnerProperty,InPropertyHandle,OwnerArrayProperty](FName InPropertyName)
		{
			if (OwnerArrayProperty)
			{
				if (FSequenceActionPropertyArrayReference* MemberReference =  SequenceAction->PropertiesArrayReference.Find(OwnerProperty->GetFName()))
				{
					return MemberReference->ArrayElementReference.Contains(InPropertyHandle->GetArrayIndex());
				}
			}
			if (SequenceAction->PropertiesReference.Contains(OwnerProperty->GetFName()))
			{
				return true;
			}
			return false;
		};

		auto OnNewFunctionBindingCreated = [](UEdGraph* InFunctionGraph, UFunction* InFunction)
		{
			// Ensure newly created function is thread safe 
		};

		auto CurrentBindingText = [SequenceAction, OwnerProperty,NodeProperty,OwnerArrayProperty,InPropertyHandle,Blueprint,OuterObjects]()
		{
			void* Data = nullptr;
			const FPropertyAccess::Result Result = InPropertyHandle->GetValueData(Data);
			if(Result == FPropertyAccess::Success && OuterObjects.Num() == 1)
			{
				if (OwnerArrayProperty)
				{
					if (FSequenceActionPropertyArrayReference* MemberReference = SequenceAction->PropertiesArrayReference.Find(OwnerProperty->GetFName()))
					{
						if (FMemberReference* ArrayMemberReference = MemberReference->ArrayElementReference.Find(InPropertyHandle->GetArrayIndex()))
						{
							if(FProperty* Property = ArrayMemberReference->ResolveMember<FProperty>(Blueprint->SkeletonGeneratedClass))
							{
								return FText::FromName(Property->GetFName());
							}
							else
							{
								return FText::FromName(ArrayMemberReference->GetMemberName());
							}
						}
					}
					else
					{
						return FText::FromName(NAME_None);
					}
				}
				else if (FMemberReference* MemberReference = SequenceAction->PropertiesReference.Find(OwnerProperty->GetFName()))
				{
					if(FProperty* Property = MemberReference->ResolveMember<FProperty>(Blueprint->SkeletonGeneratedClass))
					{
						return FText::FromName(Property->GetFName());
					}
					else
					{
						return FText::FromName(MemberReference->GetMemberName());
					}
				}
			}
			else if(Result == FPropertyAccess::MultipleValues || Result == FPropertyAccess::Success)
			{
				return NSLOCTEXT("RedUELegacy","MultipleValues", "Multiple Values");
			}

			return FText::FromName(NAME_None);
		};

		auto CurrentBindingToolTipText = [InPropertyHandle,SequenceAction, OwnerProperty,NodeProperty,OwnerArrayProperty,Blueprint,OuterObjects]()
		{
			void* StructData = nullptr;
			const FPropertyAccess::Result Result = InPropertyHandle->GetValueData(StructData);
			if(Result == FPropertyAccess::Success)
			{
				if(Result == FPropertyAccess::Success && OuterObjects.Num() == 1)
				{
					if (OwnerArrayProperty)
					{
						if (FSequenceActionPropertyArrayReference* MemberReference = SequenceAction->PropertiesArrayReference.Find(OwnerProperty->GetFName()))
						{
							if (FMemberReference* ArrayMemberReference = MemberReference->ArrayElementReference.Find(InPropertyHandle->GetArrayIndex()))
							{
								if(FProperty* Property = ArrayMemberReference->ResolveMember<FProperty>(Blueprint->SkeletonGeneratedClass))
								{
									return FText::FromName(Property->GetFName());
								}
								else
								{
									return FText::FromName(ArrayMemberReference->GetMemberName());
								}
							}
						}
						else
						{
							return FText::FromName(NAME_None);
						}
					}
					else if (FMemberReference* MemberReference = SequenceAction->PropertiesReference.Find(OwnerProperty->GetFName()))
					{
						if(FProperty* Property = MemberReference->ResolveMember<FProperty>(Blueprint->SkeletonGeneratedClass))
						{
							return FText::FromName(Property->GetFName());
						}
						else
						{
							return FText::FromName(MemberReference->GetMemberName());
						}
					}
				}
			}
			else if(Result == FPropertyAccess::MultipleValues)
			{
				return NSLOCTEXT("RedUELegacy","MultipleValues", "Multiple Values");
			}

			return FText::GetEmpty();
		};
		auto CurrentBindingImage = [NodeProperty,OwnerArrayProperty,SequenceAction,OwnerProperty,InPropertyHandle,Blueprint]()
		{
			static FName PropertyIcon(TEXT("Kismet.VariableList.TypeIcon"));
			const UEdGraphSchema_K2* GraphSchema = GetDefault<UEdGraphSchema_K2>();
			FEdGraphPinType PinType;

			if (OwnerArrayProperty)
			{
				if (FSequenceActionPropertyArrayReference* MemberReference = SequenceAction->PropertiesArrayReference.Find(OwnerProperty->GetFName()))
				{
					if (FMemberReference* ArrayMemberReference = MemberReference->ArrayElementReference.Find(InPropertyHandle->GetArrayIndex()))
					{
						if(FProperty* Property = ArrayMemberReference->ResolveMember<FProperty>(Blueprint->SkeletonGeneratedClass))
						{
							if(GraphSchema->ConvertPropertyToPinType(Property, PinType))
							{
								return FBlueprintEditorUtils::GetIconFromPin(PinType, false);
							}
						}
					}
				}
			}
			
			if(NodeProperty != nullptr && GraphSchema->ConvertPropertyToPinType(NodeProperty, PinType))
			{
				return FBlueprintEditorUtils::GetIconFromPin(PinType, false);
			}
			else
			{
				return FAppStyle::GetBrush(PropertyIcon);
			}
		};
		auto CurrentBindingColor = [OwnerProperty,SequenceAction,InPropertyHandle]()
		{
			static FName PropertyIcon(TEXT("Kismet.VariableList.TypeIcon"));
			const UEdGraphSchema_K2* GraphSchema = GetDefault<UEdGraphSchema_K2>();
			FEdGraphPinType PinType;
			if (OwnerProperty)
			{
				if (GraphSchema->ConvertPropertyToPinType(OwnerProperty, PinType))
				{
					if( SequenceAction->PropertiesReference.Contains(OwnerProperty->GetFName()))
					{
						return GraphSchema->GetPinTypeColor(PinType);
					}
					if (FSequenceActionPropertyArrayReference* PropertyArrayReference = SequenceAction->PropertiesArrayReference.Find(OwnerProperty->GetFName()))
					{
						if (PropertyArrayReference->ArrayElementReference.Contains(InPropertyHandle->GetArrayIndex()))
						{
							return GraphSchema->GetPinTypeColor(PinType);
						}
					}
				}
			}
			
			return FAppStyle::GetSlateColor("Colors.Foreground").GetSpecifiedColor();
		};
	
		FPropertyBindingWidgetArgs Args;
		Args.Property = NodeProperty;
		Args.OnGenerateBindingName = FOnGenerateBindingName::CreateLambda(OnGenerateBindingName);
		Args.OnCanBindProperty = FOnCanBindProperty::CreateLambda(OnCanBindProperty);
		Args.OnGotoBinding = FOnGotoBinding::CreateLambda(OnGoToBinding);
		Args.OnCanGotoBinding = FOnCanGotoBinding::CreateLambda(OnCanGotoBinding);
		Args.OnCanBindFunction = FOnCanBindFunction::CreateLambda(OnCanBindFunction);
		Args.OnCanBindToClass = FOnCanBindToClass::CreateLambda([](UClass* InClass){ return true; });
		Args.OnAddBinding = FOnAddBinding::CreateLambda(OnAddBinding);
		Args.OnRemoveBinding = FOnRemoveBinding::CreateLambda(OnRemoveBinding);
		Args.OnCanRemoveBinding = FOnCanRemoveBinding::CreateLambda(CanRemoveBinding);
		Args.OnNewFunctionBindingCreated = FOnNewFunctionBindingCreated::CreateLambda(OnNewFunctionBindingCreated);
		Args.CurrentBindingText = MakeAttributeLambda(CurrentBindingText);
		Args.CurrentBindingToolTipText = MakeAttributeLambda(CurrentBindingToolTipText);
		Args.CurrentBindingImage = MakeAttributeLambda(CurrentBindingImage);
		Args.CurrentBindingColor = MakeAttributeLambda(CurrentBindingColor);
		Args.bGeneratePureBindings = true;
		Args.bAllowFunctionBindings = false;
		Args.bAllowFunctionLibraryBindings = false;
		Args.bAllowPropertyBindings = true;
		Args.bAllowNewBindings = false;
		Args.bAllowArrayElementBindings = false;
		Args.bAllowUObjectFunctions = false;
		Args.bAllowStructFunctions = false;
		Args.bAllowStructMemberBindings = false;

		IPropertyAccessEditor& PropertyAccessEditor = IModularFeatures::Get().GetModularFeature<IPropertyAccessEditor>("PropertyAccessEditor");
		TSharedPtr<SWidget> BindingWidget = PropertyAccessEditor.MakePropertyBindingWidget(Blueprint, Args);
		if(BindingWidget.IsValid())
		{
			InWidgetRow.ExtensionContent()
			[
				BindingWidget.ToSharedRef()
			];
		}
	}
	
	
	
}
