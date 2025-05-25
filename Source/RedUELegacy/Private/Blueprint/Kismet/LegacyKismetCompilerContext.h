#pragma once
#include "K2Node_SequenceAction.h"
#include "KismetCompiler.h"
#include "Kismet/Base/LegacyKismetGeneratedClass.h"

class FLegacyKismetCompilerContext:public FKismetCompilerContext
{
public:
	FLegacyKismetCompilerContext(UBlueprint* SourceSketch, FCompilerResultsLog& InMessageLog,
		const FKismetCompilerOptions& InCompilerOptions)
		: FKismetCompilerContext(SourceSketch, InMessageLog, InCompilerOptions)
	{
	}
	TArray<UK2Node_SequenceAction*> Actions;
	virtual  void CreateFunctionList() override;
protected:
	virtual void FinishCompilingClass(UClass* Class) override;
	virtual void OnNewClassSet(UBlueprintGeneratedClass* ClassToUse) override;
	virtual void SpawnNewClass(const FString& NewClassName) override;
	ULegacyKismetGeneratedClass* NewKismetGeneratedClass;
	
};

