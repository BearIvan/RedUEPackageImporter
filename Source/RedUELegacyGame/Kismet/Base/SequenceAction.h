#pragma once
#include "SequenceAction.generated.h"

#define SEQUENCE_ACTION_KISMET_ATTRIBUTE(VariableType,VariableName)\
const VariableType&Get##VariableName() const\
{\
	UObject* ObjectOuter = GetOuter();\
	FName PropertyName = #VariableName;\
	if (const FMemberReference* PropertyReference = PropertiesReference.Find(PropertyName);ObjectOuter&&PropertyReference)\
	{\
		if (FProperty*InPropertyReference = PropertyReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))\
		{\
			return *InPropertyReference->ContainerPtrToValuePtr<VariableType>(ObjectOuter);\
		}\
	}\
	return VariableName;\
}\
\
void Set##VariableName(const VariableType&InValue)\
{\
	UObject* ObjectOuter = GetOuter();\
	FName PropertyName = #VariableName;\
	if (const FMemberReference* PropertyReference = PropertiesReference.Find(PropertyName);ObjectOuter&&PropertyReference)\
	{\
		if (FProperty*InPropertyReference = PropertyReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))\
		{\
			InPropertyReference->SetValue_InContainer(ObjectOuter,&InValue);\
		}\
	}\
	VariableName = InValue;\
}
class USequenceAction;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSequenceActionDelegate);


UCLASS(EditInlineNew,DefaultToInstanced)
class REDUELEGACYGAME_API USequenceAction : public UObject
{
	GENERATED_BODY()
public:
	virtual void Construct();
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);

	UPROPERTY()
	TMap<FName,FName> InitializeDelegates;
	
	UPROPERTY()
	TMap<FName,FMemberReference> PropertiesReference;
};
