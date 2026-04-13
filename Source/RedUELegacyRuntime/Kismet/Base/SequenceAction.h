#pragma once
#include "SequenceAction.generated.h"



class ALegacyKismet;
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
class REDUELEGACYRUNTIME_API USequenceAction : public UObject
{
	GENERATED_BODY()
public:
	virtual void					Construct				();
	virtual void					BeginPlay				();
	virtual void					Tick					(float DeltaTime);
#if WITH_EDITOR
	virtual void					PostEditChangeProperty	(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
			void					ExecuteCustomLink		(int32 Index);
			ALegacyKismet*			GetOwnerKismetChecked	();
			const ALegacyKismet*	GetOwnerKismetChecked	() const;
	
	
	UPROPERTY()
	TMap<FName,FName> InitializeDelegates;
	
	UPROPERTY()
	TMap<int32,FName> InitializeCustomLinks;
	
	UPROPERTY()
	TArray<UFunction*> CustomLinks;
	
	UPROPERTY()
	TMap<FName,FMemberReference> PropertiesReference;
	
	UPROPERTY()
	TMap<FName,FSequenceActionPropertyArrayReference> PropertiesArrayReference;
protected:
	template<typename T>
	T GetVariable(FName PropertyName,const T&DefaultValue) const
	{
		auto GetValue = []<typename InType>(FProperty*InPropertyReference,auto ObjectOuter)
		{
			if constexpr (std::is_same_v<InType,float>)
			{
				if (FDoubleProperty * DoubleProperty = CastField<FDoubleProperty>(InPropertyReference))
				{
					return static_cast<float>(*DoubleProperty->ContainerPtrToValuePtr<double>(ObjectOuter));
				}
			}
			return *InPropertyReference->ContainerPtrToValuePtr<InType>(ObjectOuter);
		};
		
		UObject* ObjectOuter = GetOuter();
		if constexpr (TIsTArray_V<T>)
		{
			if (const FMemberReference* PropertyReference = PropertiesReference.Find(PropertyName);ObjectOuter&&PropertyReference)
			{
				ensure(PropertiesArrayReference.Find(PropertyName) == nullptr);
				if (FProperty*InPropertyReference = PropertyReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))
				{
					if (FArrayProperty *ArrayProperty  = CastFieldChecked<FArrayProperty>(InPropertyReference))
					{
						T Result;
						FScriptArrayHelper_InContainer ArrayHelper(ArrayProperty,ObjectOuter);
						for (int32 SubI = 0;  SubI < ArrayHelper.Num(); SubI++)
						{
							Result.Add(GetValue.template operator()<typename T::ElementType>(ArrayProperty->Inner,ArrayHelper.GetRawPtr(SubI)));
						}
						return Result;
					}
				}
			}
			else if (const FSequenceActionPropertyArrayReference* PropertyArrayReference = PropertiesArrayReference.Find(PropertyName);ObjectOuter&&PropertyArrayReference)\
			{
				T Result;
				int32 Num = FMath::Min(DefaultValue.Num(),PropertyArrayReference->ArrayElementReference.Num());
				ensure(PropertyArrayReference->ArrayElementReference.Num() == Num);
				for (int32 i = 0; i < Num; i++)
				{
					if (const FMemberReference* ArrayReference = PropertyArrayReference->ArrayElementReference.Find(i))
					{
						if (FProperty*InPropertyReference = ArrayReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))
						{
							if (FArrayProperty *ArrayProperty  = CastField<FArrayProperty>(InPropertyReference))
							{
								FScriptArrayHelper_InContainer ArrayHelper(ArrayProperty,ObjectOuter);
								for (int32 SubI = 0;  SubI < ArrayHelper.Num(); SubI++)
								{
									Result.Add(GetValue.template operator()<typename T::ElementType>(ArrayProperty->Inner,ArrayHelper.GetRawPtr(SubI)));
								}
							}
							else
							{
								Result.Add(GetValue.template operator()<typename T::ElementType>(InPropertyReference,ObjectOuter));
							}
						}
					}
					else
					{
						Result.Add(DefaultValue[i]);
					}
				}
				return Result;
			}
		}
		else
		{
			if (const FMemberReference* PropertyReference = PropertiesReference.Find(PropertyName);ObjectOuter&&PropertyReference)
			{
				ensure(PropertiesArrayReference.Find(PropertyName) == nullptr);
				if (FProperty*InPropertyReference = PropertyReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))
				{
					return GetValue.template operator()<T>(InPropertyReference,ObjectOuter);
				}
			}
		}
		return DefaultValue;
	}
	
	template<typename T,typename M>
	void SetVariable(FName PropertyName,const T& InNewValue, M&OutValue) const
	{
		UObject* ObjectOuter = GetOuter();
		auto SetValue = []<typename A>(FProperty*InPropertyReference,auto ObjectOuter,const A& InNewValue)
		{
			if constexpr (std::is_same_v<A,float>)
			{
				if (FDoubleProperty * DoubleProperty = CastField<FDoubleProperty>(InPropertyReference))
				{
					DoubleProperty->SetValue_InContainer(ObjectOuter,static_cast<double>(InNewValue));
				}
				else
				{
					InPropertyReference->SetValue_InContainer(ObjectOuter,&InNewValue);
				}
			}
			else
			{
				InPropertyReference->SetValue_InContainer(ObjectOuter,&InNewValue);
			}
		};
		
		if constexpr (TIsTArray_V<M>)
		{
			if constexpr (TIsTArray_V<T>)
			{
				ensure(PropertiesArrayReference.Find(PropertyName) == nullptr);
				if (const FMemberReference* PropertyReference = PropertiesReference.Find(PropertyName);ObjectOuter&&PropertyReference)
				{
					if (FProperty*InPropertyReference = PropertyReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))
					{
						if constexpr (std::is_same_v<typename T::ElementType,float>)
						{
							if (FArrayProperty *ArrayProperty  = CastFieldChecked<FArrayProperty>(InPropertyReference))
							{
								FScriptArrayHelper_InContainer ArrayHelper(ArrayProperty,ObjectOuter);
								ArrayHelper.Resize(InNewValue.Num());
								for (int32 SubI = 0;  SubI < ArrayHelper.Num(); SubI++)
								{
									SetValue(ArrayProperty->Inner,ArrayHelper.GetRawPtr(SubI),InNewValue[SubI]);
								}
							}
						}
						else
						{
							InPropertyReference->SetValue_InContainer(ObjectOuter,&InNewValue);
						}
					}
				}
				else if (const FSequenceActionPropertyArrayReference* PropertyArrayReference = PropertiesArrayReference.Find(PropertyName);ObjectOuter&&PropertyArrayReference)
				{
					int32 Num = FMath::Min(OutValue.Num(),PropertyArrayReference->ArrayElementReference.Num());
					ensure(PropertyArrayReference->ArrayElementReference.Num() == Num);
					for (int32 i = 0,a = 0; i < Num && a < OutValue.Num(); i++,a++)
					{
						if (const FMemberReference* ArrayReference = PropertyArrayReference->ArrayElementReference.Find(i))
						{
							if (FProperty*InPropertyReference = ArrayReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))
							{
								if (FArrayProperty *ArrayProperty  = CastField<FArrayProperty>(InPropertyReference))
								{
									FScriptArrayHelper_InContainer ArrayHelper(ArrayProperty,ObjectOuter);
									for (int32 SubI = 0;  SubI < ArrayHelper.Num() && a < OutValue.Num(); SubI++, a++)
									{
										SetValue(ArrayProperty->Inner,ArrayHelper.GetRawPtr(SubI),InNewValue[a]);
									}
								}
								else 
								{
									SetValue(InPropertyReference,ObjectOuter,InNewValue[a]);
								}
							}
						}
						else
						{
							OutValue[i] = InNewValue[a];
						}
					}
				}
				else
				{
					OutValue = InNewValue;
				}
			}
			else
			{
				if (const FSequenceActionPropertyArrayReference* PropertyArrayReference = PropertiesArrayReference.Find(PropertyName);ObjectOuter&&PropertyArrayReference)
				{
					int32 Num = FMath::Min(OutValue.Num(),PropertyArrayReference->ArrayElementReference.Num());
					ensure(PropertyArrayReference->ArrayElementReference.Num() == Num);
					for (int32 i = 0; i < Num; i++)
					{
						if (const FMemberReference* ArrayReference = PropertyArrayReference->ArrayElementReference.Find(i))
						{
							if (FProperty*InPropertyReference = ArrayReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))
							{
								if (FArrayProperty *ArrayProperty  = CastField<FArrayProperty>(InPropertyReference))\
								{
									FScriptArrayHelper_InContainer ArrayHelper(ArrayProperty,ObjectOuter);
									for (int32 SubI = 0;  SubI < ArrayHelper.Num(); SubI++)
									{
										SetValue(ArrayProperty->Inner,ArrayHelper.GetRawPtr(SubI),InNewValue);
									}
								}
								else 
								{
									SetValue(InPropertyReference,ObjectOuter,InNewValue);
								}
							}
						}
						else
						{
							OutValue[i] = InNewValue;
						}
					}
				}
			}
		}
		else
		{
			if (const FMemberReference* PropertyReference = PropertiesReference.Find(PropertyName);ObjectOuter&&PropertyReference)
			{
				ensure(PropertiesArrayReference.Find(PropertyName) == nullptr);
				if (FProperty*InPropertyReference = PropertyReference->ResolveMember<FProperty>(ObjectOuter->GetClass()))
				{
					SetValue(InPropertyReference,ObjectOuter,InNewValue);
				}
			}
			OutValue = InNewValue;
		}
		
	}
	
};
	

#define SEQUENCE_ACTION_KISMET_ATTRIBUTE(VariableName)\
decltype(VariableName) Get##VariableName() const\
{\
return GetVariable<decltype(VariableName)>(#VariableName,VariableName);\
}\
\
void Set##VariableName(auto InValue)\
{\
return SetVariable(#VariableName,InValue,VariableName);\
}\

UCLASS()
class REDUELEGACYRUNTIME_API USequenceEvent : public USequenceAction
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere,Category="Event",meta = (LegacyRead))
	bool bEnabled = true;
};