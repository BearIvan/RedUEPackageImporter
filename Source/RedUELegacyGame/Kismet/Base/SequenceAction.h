#pragma once
#include "SequenceAction.generated.h"

#define SEQUENCE_ACTION_KISMET_ATTRIBUTE(VariableType,VariableName)\
VariableType Get##VariableName() const\
{\
	UObject* ObjectOuter = GetOuter();\
	FName PropertyName = #VariableName;\
	if (const FMemberReference* PropertyReference = PropertiesReference.Find(PropertyName);ObjectOuter&&PropertyReference)\
	{\
		ensure(PropertiesArrayReference.Find(PropertyName) == nullptr);\
		if (FProperty*InPropertyReference = PropertyReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))\
		{\
			return *InPropertyReference->ContainerPtrToValuePtr<VariableType>(ObjectOuter);\
		}\
	}\
	return VariableName;\
}\
\
void Set##VariableName(VariableType InValue)\
{\
	UObject* ObjectOuter = GetOuter();\
	FName PropertyName = #VariableName;\
	ensure(PropertiesArrayReference.Find(PropertyName) == nullptr);\
	if (const FMemberReference* PropertyReference = PropertiesReference.Find(PropertyName);ObjectOuter&&PropertyReference)\
	{\
		if (FProperty*InPropertyReference = PropertyReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))\
		{\
			InPropertyReference->SetValue_InContainer(ObjectOuter,&InValue);\
		}\
	}\
	VariableName = InValue;\
}




#define SEQUENCE_ACTION_KISMET_ARRAY_ATTRIBUTE(VariableType,VariableName)\
const TArray<VariableType> Get##VariableName() const\
{\
	UObject* ObjectOuter = GetOuter();\
	FName PropertyName = #VariableName;\
	if (const FMemberReference* PropertyReference = PropertiesReference.Find(PropertyName);ObjectOuter&&PropertyReference)\
	{\
		ensure(PropertiesArrayReference.Find(PropertyName) == nullptr);\
		if (FProperty*InPropertyReference = PropertyReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))\
		{\
			return *InPropertyReference->ContainerPtrToValuePtr<TArray<VariableType>>(ObjectOuter);\
		}\
	}\
	else if (const FSequenceActionPropertyArrayReference* PropertyArrayReference = PropertiesArrayReference.Find(PropertyName);ObjectOuter&&PropertyArrayReference)\
	{\
\
		TArray<VariableType> Result;\
		int32 Num = FMath::Min(VariableName.Num(),PropertyArrayReference->ArrayElementReference.Num());\
		ensure(PropertyArrayReference->ArrayElementReference.Num() == Num);\
		for (int32 i = 0; i < Num; i++)\
		{\
			if (const FMemberReference* ArrayReference = PropertyArrayReference->ArrayElementReference.Find(i))\
			{\
				if (FProperty*InPropertyReference = ArrayReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))\
				{\
					if (FArrayProperty *ArrayProperty  = CastField<FArrayProperty>(InPropertyReference))\
					{\
						FScriptArrayHelper_InContainer ArrayHelper(ArrayProperty,ObjectOuter);\
						for (int32 SubI = 0;  SubI < ArrayHelper.Num(); SubI++)\
						{\
							Result.Add(*ArrayProperty->Inner->ContainerPtrToValuePtr<VariableType>(ArrayHelper.GetRawPtr(SubI)));\
						}\
					}\
					else\
					{\
						Result.Add(*InPropertyReference->ContainerPtrToValuePtr<VariableType>(ObjectOuter));\
					}\
				}\
			}\
			else\
			{\
				Result.Add(VariableName[i]);\
			}\
		}\
		return Result;\
	}\
	return VariableName;\
}\
\
void Set##VariableName(const TArray<VariableType>&InValue)\
{\
	UObject* ObjectOuter = GetOuter();\
	FName PropertyName = #VariableName;\
	ensure(PropertiesArrayReference.Find(PropertyName) == nullptr);\
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

USTRUCT()
struct FSequenceActionPropertyArrayReference
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<int32,FMemberReference>	ArrayElementReference;
};

UCLASS(EditInlineNew,DefaultToInstanced)
class REDUELEGACYGAME_API USequenceAction : public UObject
{
	GENERATED_BODY()
public:
	virtual void Construct();
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY()
	TMap<FName,FName> InitializeDelegates;
	
	UPROPERTY()
	TMap<FName,FMemberReference> PropertiesReference;
	
	UPROPERTY()
	TMap<FName,FSequenceActionPropertyArrayReference> PropertiesArrayReference;
};
