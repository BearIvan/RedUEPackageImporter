#include "LegacyKismetCompilerContext.h"

#include "Kismet/Base/LegacyKismet.h"
#include "Kismet/Base/SequenceAction.h"



void FLegacyKismetCompilerContext::CopyTermDefaultsToDefaultObject(UObject* DefaultObject)
{
    FKismetCompilerContext::CopyTermDefaultsToDefaultObject(DefaultObject);
    
    ALegacyKismet* LegacyKismetCDO = Cast<ALegacyKismet>(DefaultObject);
    if (!LegacyKismetCDO)
    {
        return;
    }

    LegacyKismetCDO->SequenceActions.Empty();

    TArray<UK2Node_SequenceAction*> Actions;

    TArray<UEdGraph*> AllGraphs;
    Blueprint->GetAllGraphs(AllGraphs);
    for (UEdGraph* Graph : AllGraphs)
    {
        if (Graph)
        {
            Graph->GetNodesOfClass(Actions);
        }
    }

    for (UK2Node_SequenceAction* SequenceAction : Actions)
    {
        if (!SequenceAction)
        {
            continue;
        }

        if (USequenceAction* Template = SequenceAction->Action)
        {
            USequenceAction* NewStartupAction = NewObject<USequenceAction>(
                LegacyKismetCDO, Template->GetClass(), NAME_None, RF_DefaultSubObject, Template);
            SequenceAction->FillAction(NewStartupAction, *this);
            LegacyKismetCDO->SequenceActions.Add(
                SequenceAction->NodeGuid,
                NewStartupAction);
        }
    }
}
