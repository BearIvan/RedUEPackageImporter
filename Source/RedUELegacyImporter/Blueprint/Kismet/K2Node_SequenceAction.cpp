#include "K2Node_SequenceAction.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_AssignmentStatement.h"
#include "K2Node_CallFunction.h"
#include "K2Node_Composite.h"
#include "K2Node_CustomEvent.h"
#include "KismetCompiler.h"
#include "LegacyKismetCompilerContext.h"
#include "LegacyKismetGraphEditorCommands.h"
#include "ScopedTransaction.h"
#include "ToolMenu.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Kismet/Base/SequenceAction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetNodes/SGraphNodeK2Event.h"
#include "UI/SGraphNodeK2SequenceAction.h"


/////////////////////////////////////////////////////
// FKCHandler_SequenceAction
class FKCHandler_SequenceAction : public FNodeHandlingFunctor
{
public:
	FKCHandler_SequenceAction(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext)
	{
	}

	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		
	}
};

/////////////////////////////////////////////////////
// UK2Node_SequenceAction
UK2Node_SequenceAction::UK2Node_SequenceAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Action(nullptr)
{
}

void UK2Node_SequenceAction::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
		{
			UClass* Class = *ClassIt;
			if (Class->IsChildOf(USequenceAction::StaticClass()) && !Class->HasAnyClassFlags(CLASS_Abstract))
			{
				if (Class == USequenceAction::StaticClass())
				{
					continue;
				}
				UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
				check(NodeSpawner != nullptr);
	
				auto CustomizeTimelineNodeLambda = [](UEdGraphNode* NewNode, bool bIsTemplateNode,UClass* Class)
				{
					UK2Node_SequenceAction* SequenceAction = CastChecked<UK2Node_SequenceAction>(NewNode);
					SequenceAction->Action = NewObject<USequenceAction>(SequenceAction,Class,NAME_None,RF_Transactional);
				};
		
				NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(CustomizeTimelineNodeLambda,Class);
				ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
			}
		}
		
	}
}

void UK2Node_SequenceAction::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	if (!Action)
	{
		return;
	}
	InputEventsName = NAME_None;
	UClass*ActionClass = Action->GetClass();
	
	LegacyIndexToInputPin.Empty();
	LegacyNameToInputPin.Empty();
	LegacyIndexToOutputPin.Empty();
	LegacyNameToOutputPin.Empty();
	LegacyIndexToVariableName.Empty();
	LegacyNameToVariableName.Empty();
	CustomLinkToIndex.Empty();
	for (TFieldIterator<UFunction>  FunctionIT(ActionClass); FunctionIT; ++FunctionIT)
	{
		if ( FunctionIT->HasMetaData(TEXT("KismetInput")) && (FunctionIT->FunctionFlags & FUNC_BlueprintCallable))
		{
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, FunctionIT->GetFName());
			if (FunctionIT->HasMetaData(TEXT("LegacyIndex")))
			{
				LegacyIndexToInputPin.Add(FunctionIT->GetIntMetaData(TEXT("LegacyIndex")),FunctionIT->GetFName());
			}
			if (FunctionIT->HasMetaData(TEXT("LegacyName")))
			{
				LegacyNameToInputPin.Add(FunctionIT->GetMetaData(TEXT("LegacyName")),FunctionIT->GetFName());
			}
		}
	}

	
	for (TFieldIterator<FProperty> PropertyIt(ActionClass); PropertyIt; ++PropertyIt)
	{
		if (FIntProperty* LinkProperty = CastField<FIntProperty>(*PropertyIt))
		{
			if (LinkProperty->HasMetaData(TEXT("KismetLinkCount")))
			{
				if (!ensure(LegacyIndexToOutputPin.Num() == 0 && LegacyNameToOutputPin.Num() == 0))
				{
					LegacyIndexToOutputPin.Empty();
					LegacyNameToOutputPin.Empty();
				}
				int32 LinkCount = *LinkProperty->GetPropertyValuePtr_InContainer(Action);
				for (int32 LinkIndex = 0; LinkIndex < LinkCount; ++LinkIndex)
				{
					UEdGraphPin* LinkPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec,*FString::Printf(TEXT("Link %d"),LinkIndex));
					LinkPin->PinToolTip = FString::Printf(TEXT("Link %d"),LinkIndex);
					LinkPin->PinFriendlyName = FText::AsCultureInvariant(FString::Printf(TEXT("Link %d"),LinkIndex));
					CustomLinkToIndex.Add(LinkPin->GetFName(),LinkIndex);
					LegacyIndexToOutputPin.Add(LinkIndex,LinkPin->GetFName());
				}
			}
		}
		if (FMulticastDelegateProperty* Property = CastField<FMulticastDelegateProperty>(*PropertyIt))
		{
			if (!ensure(CustomLinkToIndex.IsEmpty()))
			{
				CustomLinkToIndex.Empty();
				LegacyIndexToOutputPin.Empty();
			}
			UEdGraphPin* ExecPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, Property->GetFName());
			ExecPin->PinToolTip = Property->GetToolTipText().ToString();
			ExecPin->PinFriendlyName = Property->GetDisplayNameText();
			if (Property->HasMetaData(TEXT("LegacyIndex")))
			{
				LegacyIndexToOutputPin.Add(Property->GetIntMetaData(TEXT("LegacyIndex")),Property->GetFName());
			}
			if (Property->HasMetaData(TEXT("LegacyName")))
			{
				LegacyNameToOutputPin.Add(Property->GetMetaData(TEXT("LegacyName")),Property->GetFName());
			}
		}
		else if (PropertyIt->HasMetaData(TEXT("KismetExternalVariable")))
		{
			if (PropertyIt->HasMetaData(TEXT("LegacyIndex")))
			{
				LegacyIndexToVariableName.Add(PropertyIt->GetIntMetaData(TEXT("LegacyIndex")),PropertyIt->GetFName());
			}
			if (PropertyIt->HasMetaData(TEXT("LegacyName")))
			{
				LegacyNameToVariableName.Add(PropertyIt->GetMetaData(TEXT("LegacyName")),PropertyIt->GetFName());
			}
		}
		else if (PropertyIt->HasMetaData(TEXT("KismetEvent")))
		{
			if (!ensure(InputEventsName == NAME_None))
			{
				continue;
			}
			FArrayProperty* EventProperty = CastField<FArrayProperty>(*PropertyIt);
			if (!ensure(EventProperty))
			{
				continue;
			}
			FStructProperty* EventInnerProperty =  CastField<FStructProperty>(EventProperty->Inner);
			if (!ensure(EventInnerProperty))
			{
				continue;
			}
			if (!ensure(EventInnerProperty->Struct == TBaseStructure<FGuid>::Get()))
			{
				continue;
			}
			InputEventsName = PropertyIt->GetFName();
		}
	}
	
	if (ActionClass->IsChildOf(USequenceEvent::StaticClass()))
	{
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Delegate, UK2Node_Event::DelegateOutputName);
	}
	CreateEventPins();
	
}

void UK2Node_SequenceAction::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	
	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
	check(SourceGraph && Schema);
	bool bIsErrorFree = true;

	if (!Action)
	{
		return;
	}

	UClass* ActionClass = Action->GetClass();
	
	for (UEdGraphPin* CurrentPin : Pins)
	{
		if (CurrentPin && CurrentPin->Direction == EGPD_Input && CurrentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec&& CurrentPin->LinkedTo.Num() > 0)
		{
			UK2Node_CallFunction* const CallActionFunctionNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			CallActionFunctionNode->SetFromFunction(ActionClass->FindFunctionByName(CurrentPin->PinName));
			if (CallActionFunctionNode->GetTargetFunction() == nullptr)
			{
				const FText ClassName = FText::FromString(ActionClass->GetName());
				const FString FormattedMessage = FText::Format(
					NSLOCTEXT("RedUELegacy","SequenceActionErrorFmt", "SequenceAction: Missing function {0} from class {1} for action @@"),
					FText::FromString(CurrentPin->PinName.GetPlainNameString()),
					ClassName
				).ToString();
				
				CompilerContext.MessageLog.Error(*FormattedMessage, this);
				return;
			}
			CallActionFunctionNode->AllocateDefaultPins();

			if (UEdGraphPin* SelfPin =  Schema->FindSelfPin(*CallActionFunctionNode,EGPD_Input))
			{
				UK2Node_CallFunction* const CallGetSequenceActionNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
				CallGetSequenceActionNode->SetFromFunction(ALegacyKismet::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(ALegacyKismet, GetSequenceAction)));
				CallGetSequenceActionNode->AllocateDefaultPins();
			
				static FName NAME_SequenceActionClass = "SequenceActionClass";
				static FName NAME_ActionGuidPin = "ActionGuid";
			
				UEdGraphPin* SequenceActionClassPin = CallGetSequenceActionNode->FindPinChecked(NAME_SequenceActionClass,EGPD_Input);
				Schema->TrySetDefaultObject(*SequenceActionClassPin,ActionClass);
				UEdGraphPin* ActionGuidPin = CallGetSequenceActionNode->FindPinChecked(NAME_ActionGuidPin,EGPD_Input);
				Schema->TrySetDefaultValue(*ActionGuidPin,NodeGuid.ToString());

				UEdGraphPin* ReturnValuePin = CallGetSequenceActionNode->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue,EGPD_Output);

				
				bIsErrorFree &= Schema->TryCreateConnection(SelfPin,ReturnValuePin);
				bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *CallGetSequenceActionNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute)).CanSafeConnect();
				bIsErrorFree &= Schema->TryCreateConnection(CallGetSequenceActionNode->FindPinChecked(UEdGraphSchema_K2::PN_Then),CallActionFunctionNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute));
			}
			else
			{
				bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *CallActionFunctionNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute)).CanSafeConnect();
			}
		
		}
		if (CurrentPin && CurrentPin->Direction == EGPD_Output && CurrentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec&& CurrentPin->LinkedTo.Num() > 0)
		{
			if (CustomLinkToIndex.Num() > 0)
			{
				UK2Node_CustomEvent* CurrentCENode = CompilerContext.SpawnIntermediateNode<UK2Node_CustomEvent>(this, SourceGraph);
				CurrentCENode->CustomFunctionName = *FString::Printf(TEXT("%s_%s"), *CurrentPin->GetName(), *CompilerContext.GetGuid(this));
				CurrentCENode->AllocateDefaultPins();
				bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *CurrentCENode->FindPinChecked(UEdGraphSchema_K2::PN_Then)).CanSafeConnect();
			}
			else
			{
				if (FMulticastDelegateProperty* DelegateProperty =  CastField<FMulticastDelegateProperty>(ActionClass->FindPropertyByName(CurrentPin->PinName)))
				{
					UK2Node_CustomEvent* CurrentCENode = CompilerContext.SpawnIntermediateNode<UK2Node_CustomEvent>(this, SourceGraph);
					CurrentCENode->CustomFunctionName = *FString::Printf(TEXT("%s_%s"), *CurrentPin->GetName(), *CompilerContext.GetGuid(this));
					CurrentCENode->AllocateDefaultPins();
					bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *CurrentCENode->FindPinChecked(UEdGraphSchema_K2::PN_Then)).CanSafeConnect();
				}
			}
			
		}
	}
	if (CastField<FArrayProperty>(ActionClass->FindPropertyByName(InputEventsName)))
	{
		for (UEdGraphPin* CurrentPin : Pins)
		{
			if (CurrentPin && CurrentPin->Direction == EGPD_Input && CurrentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Delegate&& CurrentPin->LinkedTo.Num() > 0)
			{
				
				ensure(CurrentPin->LinkedTo.Num()==1);
				if (CurrentPin->LinkedTo[0])
				{
					UK2Node_SequenceAction* InSequenceAction =  Cast<UK2Node_SequenceAction>( CurrentPin->LinkedTo[0]->GetOwningNode());
					if (!InSequenceAction)
					{
						const FText NodeName = FText::FromString(CurrentPin->LinkedTo[0]->GetOwningNode()->GetName());
						const FString FormattedMessage = FText::Format(
							NSLOCTEXT("RedUELegacy","SequenceActionInputEventError0Fmt", "SequenceAction: Input event {0} link only for UK2Node_SequenceAction, now refers to node {1} for action @@"),
							FText::FromString(CurrentPin->PinName.GetPlainNameString()),
							NodeName
						).ToString();
				
						CompilerContext.MessageLog.Error(*FormattedMessage, this);
						return;
					}
					if (!InSequenceAction->Action)
					{
						
						const FText NodeName = FText::FromString(InSequenceAction->GetName());
						const FString FormattedMessage = FText::Format(
							NSLOCTEXT("RedUELegacy","SequenceActionInputEventError1Fmt", "SequenceAction: Input event {0} refers to node {1} have empty action for action @@"),
							FText::FromString(CurrentPin->PinName.GetPlainNameString()),
							NodeName
						).ToString();
				
						CompilerContext.MessageLog.Error(*FormattedMessage, this);
						return;
					}
					if (!InSequenceAction->Action->IsA<USequenceEvent>())
					{
						
						const FText NodeName = FText::FromString(InSequenceAction->GetName());
						const FString FormattedMessage = FText::Format(
							NSLOCTEXT("RedUELegacy","SequenceActionInputEventError2Fmt", "SequenceAction: Input event {0} link only for UK2Node_SequenceAction, refers to node {1} in which action is not child of SequenceEvent for action @@"),
							FText::FromString(CurrentPin->PinName.GetPlainNameString()),
							NodeName
						).ToString();
				
						CompilerContext.MessageLog.Error(*FormattedMessage, this);
						return;
					}
				}
			}
		}
	}
	
	if (!bIsErrorFree)
	{
		CompilerContext.MessageLog.Error(*NSLOCTEXT("RedUELegacy","InternalConnectionError", "SequenceAction: Internal connection error. @@").ToString(), this);
	}
	
	if (ActionClass->IsChildOf(USequenceEvent::StaticClass()))
	{
		for (UEdGraphPin* CurrentPin : Pins)
		{
			if (CurrentPin && CurrentPin->Direction == EGPD_Output && CurrentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Delegate&& CurrentPin->LinkedTo.Num() > 0)
			{
				for (UEdGraphPin* LinkedPin :  CurrentPin->LinkedTo)
				{
					if (!Cast<UK2Node_SequenceAction>(LinkedPin->GetOwningNode())&&LinkedPin->GetOwningNode())
					{
						const FText NodeName = FText::FromString(LinkedPin->GetOwningNode()->GetName());
						const FString FormattedMessage = FText::Format(
							NSLOCTEXT("RedUELegacy","SequenceActionOutputEventErrorFmt", "SequenceAction: Output event {0} link only for UK2Node_SequenceAction, refers to node {1} in which action is not child of SequenceEvent for action @@"),
							FText::FromString(CurrentPin->PinName.GetPlainNameString()),
							NodeName
						).ToString();
				
						CompilerContext.MessageLog.Error(*FormattedMessage, this);
						return;
					}
				}
			}
		}
	}
	// Create a call to factory the proxy object
	BreakAllNodeLinks();
	
}

bool UK2Node_SequenceAction::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph);
	if(Blueprint)
	{
		const UEdGraphSchema_K2* K2Schema = Cast<UEdGraphSchema_K2>(TargetGraph->GetSchema());
		check(K2Schema);

		const bool bSupportsEventGraphs = FBlueprintEditorUtils::DoesSupportEventGraphs(Blueprint);
		const bool bAllowEvents = (K2Schema->GetGraphType(TargetGraph) == GT_Ubergraph) && bSupportsEventGraphs &&
			(Blueprint->BlueprintType != BPTYPE_MacroLibrary);

		if(bAllowEvents)
		{
			return  Blueprint&& Blueprint->ParentClass&&Blueprint->ParentClass->IsChildOf(ALegacyKismet::StaticClass())&& FBlueprintEditorUtils::DoesSupportEventGraphs(Blueprint);
		}
		else
		{
			bool bCompositeOfUbberGraph = false;

			//If the composite has a ubergraph in its outer, it is allowed to have timelines
			if (bSupportsEventGraphs && K2Schema->IsCompositeGraph(TargetGraph))
			{
				while (TargetGraph)
				{
					if (UK2Node_Composite* Composite = Cast<UK2Node_Composite>(TargetGraph->GetOuter()))
					{
						TargetGraph = Cast<UEdGraph>(Composite->GetOuter());
					}
					else if (K2Schema->GetGraphType(TargetGraph) == GT_Ubergraph)
					{
						bCompositeOfUbberGraph = true;
						break;
					}
					else
					{
						TargetGraph = Cast<UEdGraph>(TargetGraph->GetOuter());
					}
				}
			}
			return bCompositeOfUbberGraph ? (Blueprint&& Blueprint->ParentClass&&Blueprint->ParentClass->IsChildOf(ALegacyKismet::StaticClass())&& FBlueprintEditorUtils::DoesSupportEventGraphs(Blueprint)) : false;
		}
	}
	return false;
}

FText UK2Node_SequenceAction::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (Action)
	{
		return FText::FromString(Action->GetClass()->GetName());
	}
	return Super::GetNodeTitle(TitleType);
}

bool UK2Node_SequenceAction::IsNodeRootSet() const
{
	return true;
}

TSharedPtr<SGraphNode> UK2Node_SequenceAction::CreateVisualWidget()
{
	return SNew(SGraphNodeK2SequenceAction, this);
}

void UK2Node_SequenceAction::FillAction(USequenceAction* ToAction, FKismetCompilerContext& CompilerContext)
{
	if (!ToAction || !Action)
	{
		return;
	}

	ToAction->InitializeDelegates.Empty();
	ToAction->InitializeCustomLinks.Empty();

	for (UEdGraphPin* CurrentPin : Pins)
	{
		if (!CurrentPin || CurrentPin->Direction != EGPD_Output || CurrentPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec || CurrentPin->LinkedTo.Num() == 0)
		{
			continue;
		}

		const FName CustomFunctionName = *FString::Printf(TEXT("%s_%s"), *CurrentPin->GetName(), *CompilerContext.GetGuid(this));
		ToAction->InitializeDelegates.Add(CurrentPin->PinName, CustomFunctionName);

		if (const int32* LinkIndex = CustomLinkToIndex.Find(CurrentPin->PinName))
		{
			ToAction->InitializeCustomLinks.Add(*LinkIndex, CustomFunctionName);
		}
	}

	if (FArrayProperty* InputEventsProperty = CastField<FArrayProperty>(ToAction->GetClass()->FindPropertyByName(InputEventsName)))
	{
		FScriptArrayHelper_InContainer InputEventsArray(InputEventsProperty, ToAction);
		InputEventsArray.EmptyValues();

		TSet<FGuid> InputEventsGuids;
		for (UEdGraphPin* CurrentPin : Pins)
		{
			if (!CurrentPin || CurrentPin->Direction != EGPD_Input || CurrentPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Delegate || CurrentPin->LinkedTo.Num() == 0)
			{
				continue;
			}

			ensure(CurrentPin->LinkedTo.Num() == 1);
			if (UEdGraphPin* LinkedPin = CurrentPin->LinkedTo[0])
			{
				if (UK2Node_SequenceAction* InSequenceAction = Cast<UK2Node_SequenceAction>(LinkedPin->GetOwningNode()))
				{
					if (InputEventsGuids.Contains(InSequenceAction->NodeGuid))
					{
						continue;
					}

					InputEventsGuids.Add(InSequenceAction->NodeGuid);
					FGuid* ActionGuid = reinterpret_cast<FGuid*>(InputEventsArray.GetRawPtr(InputEventsArray.AddValue()));
					*ActionGuid = InSequenceAction->NodeGuid;
				}
			}
		}
	}
}

bool UK2Node_SequenceAction::CanAddEventPin() const
{
	if (!Action)
	{
		return false;
	}
	return InputEventsName != NAME_None;
}

void UK2Node_SequenceAction::AddEventPin()
{
	Modify();
	CreateEventPin();


	GetGraph()->NotifyGraphChanged();
	
					
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(GetGraph());
	if (ensure(Blueprint))
	{
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	}
}

void UK2Node_SequenceAction::CreateEventPins()
{
	if (!CanAddEventPin())
	{
		return;
	}
	
	int32 OldEventPinsCount = EventPinsCount;
	EventPinsCount = 0;
	for (int32 i = 0; i < OldEventPinsCount; ++i)
	{
		AddEventPin();
	}
}

void UK2Node_SequenceAction::CreateEventPin()
{
	UEdGraphPin* NewPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Delegate, *FString::Printf(TEXT("Event %d"), EventPinsCount));
	if (NewPin->PinName.IsNone())
	{
		NewPin->PinName = CreateUniquePinName(TEXT("Event"));
		NewPin->PinFriendlyName = FText::FromString(TEXT(" "));
	}
	EventPinsCount++;
}

void UK2Node_SequenceAction::RemoveEventPin(const UEdGraphPin* InGraphPin)
{
	const FScopedTransaction Transaction( NSLOCTEXT("RedUELegacy","DeleteSequenceActionEvent_Transaction", "Delete Event Input") );
	Modify();

	TArray<class UEdGraphPin*> EventPins;
	GetEventPins(EventPins);

	for (int32 OutputIndex = 0; OutputIndex < EventPins.Num(); OutputIndex++)
	{
		if (InGraphPin == EventPins[OutputIndex])
		{
			EventPins[OutputIndex]->MarkAsGarbage();
			Pins.Remove(EventPins[OutputIndex]);
			EventPinsCount--;
			break;
		}
	}

	GetEventPins(EventPins);
	for (int32 OutputIndex = 0; OutputIndex < EventPins.Num(); OutputIndex++)
	{
		EventPins[OutputIndex]->PinName = *FString::Printf(TEXT("Event %d"),OutputIndex);
	}
	GetGraph()->NotifyGraphChanged();
	
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(GetGraph());
	if (ensure(Blueprint))
	{
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	}
}

void UK2Node_SequenceAction::GetEventPins(TArray<UEdGraphPin*>& OutPins)
{
	OutPins.Empty();
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Input&&Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Delegate)
		{
			OutPins.Add(Pin);
		}
	}
}

UEdGraphPin* UK2Node_SequenceAction::GetEventOutput()
{
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output&&Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Delegate)
		{
			return Pin;
		}
	}
	return nullptr;
}

void UK2Node_SequenceAction::OnRemoveEventPin(const UEdGraphPin* InGraphPin)
{
	if (InGraphPin && InGraphPin->Direction == EGPD_Input&&InGraphPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Delegate)
	{
		RemoveEventPin(InGraphPin);
	}
}

void UK2Node_SequenceAction::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);
	
	const UEdGraph* CurrentGraph = Context->Graph;
	const UEdGraphNode* InGraphNode = Context->Node;
	const UEdGraphPin* InGraphPin = Context->Pin;
	const bool bIsDebugging = Context->bIsDebugging;
	
	if (!bIsDebugging && InGraphPin && InGraphNode == this)
	{
		if (InGraphPin->Direction == EGPD_Input && InGraphPin->PinType.PinCategory  == UEdGraphSchema_K2::PC_Delegate)
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("EditPin");
			Section.AddMenuEntry(
			"RemoveEventPin",
			NSLOCTEXT("ReUELegacy","RemoveEventPin", "Remove event pin" ),
			NSLOCTEXT("ReUELegacy","RemoveEventPinTooltip", "" ),FSlateIcon(),FUIAction(
			FExecuteAction::CreateUObject( const_cast<UK2Node_SequenceAction*>(this), &UK2Node_SequenceAction::OnRemoveEventPin,InGraphPin))
			);
		}
	}
	
}

class FNodeHandlingFunctor* UK2Node_SequenceAction::CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const
{
	return new FKCHandler_SequenceAction(CompilerContext);
}



