#include "SequenceActionDetails.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailGroup.h"
#include "ObjectEditorUtils.h"
#include "Blueprint/Kismet/K2Node_SequenceAction.h"
#include "Kismet/Base/SequenceAction.h"

TSharedRef<IDetailCustomization> FSequenceActionDetails::MakeInstance()
{
	return MakeShareable(new FSequenceActionDetails());
}

void FSequenceActionDetails::CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder)
{
	TArray< TWeakObjectPtr<UObject> > SelectedObjectsList;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjectsList);

	// In the case of property row generators, we dont have a details view, so we can skip this work
	if(DetailBuilder.GetDetailsView() == nullptr)
	{
		return;
	}

	UClass* SequenceActionClass = nullptr;
	USequenceAction* FirstSequenceAction = nullptr;
	for (TWeakObjectPtr<UObject> SelectionObject: SelectedObjectsList)
	{
		UK2Node_SequenceAction* SequenceActionNode = Cast<UK2Node_SequenceAction>(SelectionObject.Get());
		if (SequenceActionNode == nullptr)
		{
			return;
		}
		USequenceAction* SequenceAction = Cast<USequenceAction>(SequenceActionNode->Action);
		if (SequenceAction == nullptr)
		{
			return;
		}
		if (SequenceActionClass == nullptr)
		{
			SequenceActionClass = SequenceAction->GetClass();
			FirstSequenceAction = SequenceAction;
		}
		else if (SequenceAction->GetClass() != SequenceActionClass)
		{
			return;
		}
		
	}



	for (TFieldIterator<FProperty>  PropertyIt(SequenceActionClass); PropertyIt; ++PropertyIt)
	{
		if (PropertyIt->HasMetaData(TEXT("KismetExternalVariable")))
		{
		
			TSharedPtr<IPropertyHandle> TargetPropertyHandle = DetailBuilder.GetProperty(PropertyIt->GetFName(),SequenceActionClass);
			FProperty* TargetProperty = *PropertyIt;
			{
				
				// if customized, do not do anything
				if (TargetPropertyHandle->IsCustomized())
				{
					continue;
				}
				
				
				// sometimes because of order of customization
				// this gets called first for the node you'd like to customize
				// then the above statement won't work
				// so you can mark certain property to have meta data "CustomizeProperty"
				// which will trigger below statement
				IDetailCategoryBuilder& CurrentCategory = DetailBuilder.EditCategory(FObjectEditorUtils::GetCategoryFName(TargetProperty));
					
				bool bShowChildren = true;
				{
					
					IDetailPropertyRow& PropertyRow = CurrentCategory.AddProperty(TargetPropertyHandle);

					TSharedPtr<SWidget> NameWidget;
					TSharedPtr<SWidget> ValueWidget;
					FDetailWidgetRow Row;
					PropertyRow.GetDefaultWidgets(NameWidget, ValueWidget, Row);
				
				
					if (FirstSequenceAction->PropertiesReference.Contains(TargetProperty->GetFName()))
					{
						bShowChildren = false;
					}
				
					ValueWidget->SetVisibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSPLambda(this, [bShowChildren,Num = SelectedObjectsList.Num()]()
					{
						if (!bShowChildren)
						{
							return EVisibility::Hidden;
						}
						return Num > 1?  EVisibility::Hidden : EVisibility::Visible;
					})));
				
					
					PropertyRow.ShowPropertyButtons(bShowChildren);
					PropertyRow.CustomWidget(bShowChildren )
					.NameContent()
					.MinDesiredWidth(Row.NameWidget.MinWidth)
					.MaxDesiredWidth(Row.NameWidget.MaxWidth)
					[
						NameWidget.ToSharedRef()
					]
					.ValueContent()
					.MinDesiredWidth(Row.ValueWidget.MinWidth)
					.MaxDesiredWidth(Row.ValueWidget.MaxWidth)
					[
						ValueWidget.ToSharedRef()
					];
					
				}
				TSharedPtr<IPropertyHandleArray> TargetArrayPropertyHandle = TargetPropertyHandle->AsArray();
				if (bShowChildren && TargetArrayPropertyHandle && FirstSequenceAction->PropertiesArrayReference.Find(TargetProperty->GetFName()))
				{
					uint32 CountElement;
					if (TargetArrayPropertyHandle->GetNumElements(CountElement) == FPropertyAccess::Success)
					{
						for (uint32  i = 0 ;i < CountElement;i++)
						{
							TSharedPtr<IPropertyHandle> TargetArrayElementPropertyHandle = TargetArrayPropertyHandle->GetElement(i);
							if (TargetArrayElementPropertyHandle->IsCustomized())
							{
								continue;
							}
							
							
							IDetailPropertyRow& PropertyRow = CurrentCategory.AddProperty(TargetArrayElementPropertyHandle);
							
							//CurrentCategory.
							bShowChildren = true;
							bool bShowPropertyButtons = true;
							if (FSequenceActionPropertyArrayReference* PropertyArrayReference= FirstSequenceAction->PropertiesArrayReference.Find(TargetProperty->GetFName()) )
							{
								if (PropertyArrayReference->ArrayElementReference.Contains(TargetArrayElementPropertyHandle->GetArrayIndex()))
								{
									bShowChildren = false;
								}
								bShowPropertyButtons = PropertyArrayReference->ArrayElementReference.Num() == 0;
							}
							

							TSharedPtr<SWidget> NameWidget;
							TSharedPtr<SWidget> ValueWidget;
							FDetailWidgetRow Row;
							PropertyRow.GetDefaultWidgets(NameWidget, ValueWidget, Row);
							
							ValueWidget->SetVisibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSPLambda(this, [bShowChildren,Num = SelectedObjectsList.Num()]()
							{
								if (!bShowChildren)
								{
									return EVisibility::Hidden;
								}
								return Num > 1?  EVisibility::Hidden : EVisibility::Visible;
							})));
								
							FString PropertyName = FString::Printf(TEXT("%s[%d]"),*TargetProperty->GetName() , TargetArrayElementPropertyHandle->GetArrayIndex()) ;
							PropertyRow.ShowPropertyButtons(bShowPropertyButtons);
							PropertyRow.CustomWidget(bShowChildren)
							.NameContent()
							.MinDesiredWidth(Row.NameWidget.MinWidth)
							.MaxDesiredWidth(Row.NameWidget.MaxWidth)
							[
								SNew(STextBlock)
								.Text(FText::AsCultureInvariant(PropertyName))
								.Font(IDetailLayoutBuilder::GetDetailFont())
							]
							.ValueContent()
							.MinDesiredWidth(Row.ValueWidget.MinWidth)
							.MaxDesiredWidth(Row.ValueWidget.MaxWidth)
							[
								ValueWidget.ToSharedRef()
							];
							
						}
					}
					
				
				}
				
			}
		}
	}
}
