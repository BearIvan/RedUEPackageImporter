#include "LegacyKismetCompilerContext.h"

#include "Kismet/Base/SequenceAction.h"
#include "Kismet2/KismetReinstanceUtilities.h"

void FLegacyKismetCompilerContext::CreateFunctionList()
{
    ensure(Actions.IsEmpty());
    FKismetCompilerContext::CreateFunctionList();
    
}

void FLegacyKismetCompilerContext::FinishCompilingClass(UClass* Class)
{
    FKismetCompilerContext::FinishCompilingClass(Class);

    if (!ensure(NewKismetGeneratedClass))
    {
        return;
    }
    
    for (UK2Node_SequenceAction* SequenceAction:Actions)
    {
        if (USequenceAction* Template = SequenceAction->Action)
        {
            NewKismetGeneratedClass->StartupActions.Add(SequenceAction->NodeGuid,NewObject<USequenceAction>(NewKismetGeneratedClass,Template->GetClass(),NAME_None,RF_NoFlags,Template));
        }
    }
    
    Actions.Empty();
}

void FLegacyKismetCompilerContext::OnNewClassSet(UBlueprintGeneratedClass* ClassToUse)
{
    FKismetCompilerContext::OnNewClassSet(ClassToUse);
	NewKismetGeneratedClass = Cast<ULegacyKismetGeneratedClass>(ClassToUse);
}

void FLegacyKismetCompilerContext::SpawnNewClass(const FString& NewClassName)
{
    NewKismetGeneratedClass = FindObject<ULegacyKismetGeneratedClass>(Blueprint->GetOutermost(), *NewClassName);

    if ( NewKismetGeneratedClass == NULL )
    {
        NewKismetGeneratedClass = NewObject<ULegacyKismetGeneratedClass>(Blueprint->GetOutermost(), FName(*NewClassName), RF_Public | RF_Transactional);
    }
    else
    {
        // Already existed, but wasn't linked in the Blueprint yet due to load ordering issues
        FBlueprintCompileReinstancer::Create(NewKismetGeneratedClass);
    }
    NewClass = NewKismetGeneratedClass;
}