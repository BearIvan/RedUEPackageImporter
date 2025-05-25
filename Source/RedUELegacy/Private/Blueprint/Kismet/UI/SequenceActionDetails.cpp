#include "SequenceActionDetails.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
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
	
	UK2Node_SequenceAction* SequenceActionNode = Cast<UK2Node_SequenceAction>(SelectedObjectsList[0].Get());
	if (SequenceActionNode == nullptr)
	{
		return;
	}
	USequenceAction* SequenceAction = Cast<USequenceAction>(SequenceActionNode->Action);
	if (SequenceAction == nullptr)
	{
		return;
	}


	for (TFieldIterator<FProperty>  PropertyIt(SequenceAction->GetClass()); PropertyIt; ++PropertyIt)
	{
		if (PropertyIt->HasMetaData(TEXT("KismetExternalVariable")))
		{
		
			TSharedPtr<IPropertyHandle> TargetPropertyHandle = DetailBuilder.GetProperty(PropertyIt->GetFName(),SequenceAction->GetClass());
			FProperty* TargetProperty = *PropertyIt;
			IDetailCategoryBuilder& CurrentCategory = DetailBuilder.EditCategory(FObjectEditorUtils::GetCategoryFName(TargetProperty));

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

			
				IDetailPropertyRow& PropertyRow = CurrentCategory.AddProperty(TargetPropertyHandle);

				TSharedPtr<SWidget> NameWidget;
				TSharedPtr<SWidget> ValueWidget;
				FDetailWidgetRow Row;
				PropertyRow.GetDefaultWidgets(NameWidget, ValueWidget, Row);

				ValueWidget->SetVisibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSPLambda(this, [SequenceAction,TargetProperty,Num = SelectedObjectsList.Num()]()
				{
					if (SequenceAction->PropertiesReference.Contains(TargetProperty->GetFName()) || Num > 1)
					{
						return EVisibility::Hidden;
					}
					return EVisibility::Visible;
				})));
				const bool bShowChildren = !SequenceAction->PropertiesReference.Contains(TargetProperty->GetFName());
						
				PropertyRow.CustomWidget(bShowChildren)
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
		}
	}
}
