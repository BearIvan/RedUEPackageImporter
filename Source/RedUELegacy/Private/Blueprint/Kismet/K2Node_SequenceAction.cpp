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
#include "Kismet/Base/LegacyKismet.h"
#include "Kismet/Base/SequenceAction.h"
#include "Kismet2/BlueprintEditorUtils.h"



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
	: Super(ObjectInitializer)
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
	
	UClass*ActionClass = Action->GetClass();
	
	for (TFieldIterator<UFunction>  FunctionIT(ActionClass); FunctionIT; ++FunctionIT)
	{
		if ( FunctionIT->HasMetaData(TEXT("KismetInput")) && (FunctionIT->FunctionFlags & FUNC_BlueprintCallable))
		{
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, FunctionIT->GetFName());
			if (FunctionIT->HasMetaData(TEXT("LegacyIndex")))
			{
				LegacyIndexToInputPin.Add(FunctionIT->GetIntMetaData(TEXT("LegacyIndex")),FunctionIT->GetFName());
			}
		}
	}

	for (TFieldIterator<FProperty> PropertyIt(ActionClass); PropertyIt; ++PropertyIt)
	{
		if (FMulticastDelegateProperty* Property = CastField<FMulticastDelegateProperty>(*PropertyIt))
		{
			UEdGraphPin* ExecPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, Property->GetFName());
			ExecPin->PinToolTip = Property->GetToolTipText().ToString();
			ExecPin->PinFriendlyName = Property->GetDisplayNameText();
			if (Property->HasMetaData(TEXT("LegacyIndex")))
			{
				LegacyIndexToOutputPin.Add(Property->GetIntMetaData(TEXT("LegacyIndex")),Property->GetFName());
			}
		}
		else if (PropertyIt->HasMetaData(TEXT("KismetExternalVariable")))
		{
			if (PropertyIt->HasMetaData(TEXT("LegacyIndex")))
			{
				LegacyIndexToVariableName.Add(PropertyIt->GetIntMetaData(TEXT("LegacyIndex")),PropertyIt->GetFName());
			}
		}
	}

	
}

void UK2Node_SequenceAction::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	static_cast<FLegacyKismetCompilerContext&>(CompilerContext).Actions.Add(this);
	
	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
	check(SourceGraph && Schema);
	bool bIsErrorFree = true;

	if (!Action)
	{
		return;
	}
	Action->InitializeDelegates.Empty();
	
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
			if (FMulticastDelegateProperty* DelegateProperty =  CastField<FMulticastDelegateProperty>(ActionClass->FindPropertyByName(CurrentPin->PinName)))
			{
				UK2Node_CustomEvent* CurrentCENode = CompilerContext.SpawnIntermediateNode<UK2Node_CustomEvent>(this, SourceGraph);
				CurrentCENode->CustomFunctionName = *FString::Printf(TEXT("%s_%s"), *CurrentPin->GetName(), *CompilerContext.GetGuid(this));
				CurrentCENode->AllocateDefaultPins();
				bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *CurrentCENode->FindPinChecked(UEdGraphSchema_K2::PN_Then)).CanSafeConnect();
				Action->InitializeDelegates.Add(CurrentPin->PinName,CurrentCENode->CustomFunctionName);
			}
		}
	}
	
	if (!bIsErrorFree)
	{
		CompilerContext.MessageLog.Error(*NSLOCTEXT("RedUELegacy","InternalConnectionError", "SequenceAction: Internal connection error. @@").ToString(), this);
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

class FNodeHandlingFunctor* UK2Node_SequenceAction::CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const
{
	return new FKCHandler_SequenceAction(CompilerContext);
}



