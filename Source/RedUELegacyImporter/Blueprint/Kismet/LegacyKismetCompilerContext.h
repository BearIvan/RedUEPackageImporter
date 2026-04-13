#pragma once
#include "K2Node_SequenceAction.h"
#include "KismetCompiler.h"

class FLegacyKismetCompilerContext:public FKismetCompilerContext
{
public:
	typedef FKismetCompilerContext Super;
	FLegacyKismetCompilerContext(UBlueprint* SourceSketch, FCompilerResultsLog& InMessageLog,
		const FKismetCompilerOptions& InCompilerOptions)
		: FKismetCompilerContext(SourceSketch, InMessageLog, InCompilerOptions)
	{
	}
protected:
	virtual void CopyTermDefaultsToDefaultObject(UObject* DefaultObject) override;
};

