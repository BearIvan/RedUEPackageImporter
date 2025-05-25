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
		
		auto OnCanBindProperty = [NodeProperty](FProperty* InProperty)
		{
			IPropertyAccessEditor& PropertyAccessEditor = IModularFeatures::Get().GetModularFeature<IPropertyAccessEditor>("PropertyAccessEditor");
			return NodeProperty && PropertyAccessEditor.GetPropertyCompatibility(InProperty, NodeProperty) != EPropertyAccessCompatibility::Incompatible;\
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

		auto OnAddBinding = [InPropertyHandle, Blueprint,NodeProperty,SequenceAction](FName InPropertyName, const TArray<FBindingChainElement>& InBindingChain)
		{
			void* Data = nullptr;
			const FPropertyAccess::Result Result = InPropertyHandle->GetValueData(Data);
			if(Result == FPropertyAccess::Success)
			{
				SequenceAction->PreEditChange(nullptr);
				
				FMemberReference& MemberReference =  SequenceAction->PropertiesReference.FindOrAdd(NodeProperty->GetFName());
				FProperty* InProperty = InBindingChain[0].Field.Get<FProperty>();
				UClass* OwnerClass = InProperty ? InProperty->GetOwnerClass() : nullptr;
				bool bSelfContext = false;
				if(OwnerClass != nullptr)
				{
					bSelfContext = (Blueprint->GeneratedClass != nullptr && Blueprint->GeneratedClass->IsChildOf(OwnerClass)) ||
									(Blueprint->SkeletonGeneratedClass != nullptr && Blueprint->SkeletonGeneratedClass->IsChildOf(OwnerClass));
				}
				MemberReference.SetFromField<FProperty>(InProperty, bSelfContext);

				SequenceAction->PostEditChange();
				SequenceAction->Modify();
				FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
			}
		};

		auto OnRemoveBinding = [SequenceAction,NodeProperty,Blueprint](FName InPropertyName)
		{
			SequenceAction->PreEditChange(nullptr);
			SequenceAction->PropertiesReference.Remove(NodeProperty->GetFName());
			SequenceAction->PostEditChange();
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
		};

		auto CanRemoveBinding = [SequenceAction,NodeProperty](FName InPropertyName)
		{
			return SequenceAction->PropertiesReference.Contains(NodeProperty->GetFName());
		};

		auto OnNewFunctionBindingCreated = [](UEdGraph* InFunctionGraph, UFunction* InFunction)
		{
			// Ensure newly created function is thread safe 
		};

		auto CurrentBindingText = [SequenceAction, NodeProperty,InPropertyHandle,Blueprint,OuterObjects]()
		{
			void* Data = nullptr;
			const FPropertyAccess::Result Result = InPropertyHandle->GetValueData(Data);
			if(Result == FPropertyAccess::Success && OuterObjects.Num() == 1)
			{
				if (FMemberReference* MemberReference = SequenceAction->PropertiesReference.Find(NodeProperty->GetFName()))
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

		auto CurrentBindingToolTipText = [InPropertyHandle,SequenceAction,NodeProperty,Blueprint]()
		{
			void* StructData = nullptr;
			const FPropertyAccess::Result Result = InPropertyHandle->GetValueData(StructData);
			if(Result == FPropertyAccess::Success)
			{
				if (FMemberReference* MemberReference = SequenceAction->PropertiesReference.Find(NodeProperty->GetFName()))
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
			else if(Result == FPropertyAccess::MultipleValues)
			{
				return NSLOCTEXT("RedUELegacy","MultipleValues", "Multiple Values");
			}

			return FText::GetEmpty();
		};
		auto CurrentBindingImage = [NodeProperty]()
		{
			static FName PropertyIcon(TEXT("Kismet.VariableList.TypeIcon"));
			const UEdGraphSchema_K2* GraphSchema = GetDefault<UEdGraphSchema_K2>();
			FEdGraphPinType PinType;
			if(NodeProperty != nullptr && GraphSchema->ConvertPropertyToPinType(NodeProperty, PinType))
			{
				return FBlueprintEditorUtils::GetIconFromPin(PinType, false);
			}
			else
			{
				return FAppStyle::GetBrush(PropertyIcon);
			}
		};
		auto CurrentBindingColor = [NodeProperty,SequenceAction]()
		{
			static FName PropertyIcon(TEXT("Kismet.VariableList.TypeIcon"));
			const UEdGraphSchema_K2* GraphSchema = GetDefault<UEdGraphSchema_K2>();
			FEdGraphPinType PinType;
			if(NodeProperty != nullptr && SequenceAction->PropertiesReference.Contains(NodeProperty->GetFName()) && GraphSchema->ConvertPropertyToPinType(NodeProperty, PinType))
			{
				return GraphSchema->GetPinTypeColor(PinType);
			}
			else
			{
				return FAppStyle::GetSlateColor("Colors.Foreground").GetSpecifiedColor();
			}
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
