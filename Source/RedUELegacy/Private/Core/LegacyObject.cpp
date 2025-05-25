#include "..\..\Public\Core\LegacyObject.h"

#include <zconf.h>

#include "ToolContextInterfaces.h"
#include "..\..\Public\Core\RedUELegacyArchive.h"
#include "Core/LegacyPackage.h"
#include "Core/LegacyTypeInfo.h"
#include "Core/RedUELegacySubsystem.h"

FLegacyRotator::operator FRotator()
{
	FRotator Rotation;
	Rotation.Yaw =		Yaw*(180.f / 32768.f);
	Rotation.Roll =		Roll*(180.f / 32768.f);
	Rotation.Pitch =	Pitch*(180.f / 32768.f);
	return Rotation;
}

FLegacyRotator& FLegacyRotator::operator=(const FRotator3f& Rotator)
{
	Yaw = FMath::FloorToInt32(Rotator.Yaw/180.f * 32768.f);
	Roll = FMath::FloorToInt32(Rotator.Roll/180.f * 32768.f);
	Pitch = FMath::FloorToInt32(Rotator.Pitch/180.f * 32768.f);
	return *this;
}

FLegacyRotator::operator FRotator3f()
{
	FRotator3f Rotation;
	Rotation.Yaw =		Yaw*(180.f / 32768.f);
	Rotation.Roll =		Roll*(180.f / 32768.f);
	Rotation.Pitch =	Pitch*(180.f / 32768.f);
	return Rotation;
}

bool FLegacyRotator::Serialize(FArchive& Ar)
{
	Ar << Pitch << Yaw << Roll;
	return true;
}

void ULegacyObject::LegacyPostLoad()
{
}

struct FPushedState
{
    class UObject* State;
    class UObject* Node;
    uint32 Offset;
    friend FArchive& operator<<(FArchive& Ar, FPushedState& V)
    {
        Ar<<V.State<< V.Node<< V.Offset;
        return Ar;
    }
};

void ULegacyObject::LegacySerialize(FRedUELegacyArchive& Ar)
{
	constexpr int64 RF_HasStack = 0x0200000000000000ull;
	// if (GetLegacyFullName() == TEXT("TheWorld.PersistentLevel.XReactiveSkeletalMeshActor2.XSkeletalMeshComponent0"))
	// {
	// 	__nop();
	// }
    if (LegacyObjectFlags&RF_HasStack)
    {
        int32 Node;
        UStruct*StateNode;
        Ar<<Node<< StateNode;
        if(Ar.LegacyVer < 691)
        {
            int64 ProbeMask;
            Ar<<ProbeMask;
        }
        else
        {
            int32 ProbeMask;
            Ar << ProbeMask;
        }
        if (Ar.LegacyVer < 566)
        {
            int32 ProbeMask;
            Ar << ProbeMask;
        }
        else
        {
            int16 ProbeMask;
            Ar << ProbeMask;
        }
        TArray< FPushedState> Stats;
        Ar<<Stats;
        if(Node!=0)
        {
            int32 Offset;
            Ar << Offset;
        }
    }
    if(IsAComponent())
    {
        LegacySerializeComponent(Ar);
    }
    
    if (Ar.LegacyVer >= 322)
    {
        Ar << NetIndex;
    }
    
    if(GetClass()->IsChildOf<ULegacyClass>())
    {
        return;
    }
	PreLegacySerializeUnrealProps(Ar);
    LegacySerializeUnrealProps(GetClass(),this,Ar);
    
}


enum class EFLegacyPropType:int32
{
    ByteProperty = 1,
    IntProperty,
    BoolProperty,
    FloatProperty,
    ObjectProperty,
    NameProperty,
    StringProperty,		// NAME_DelegateProperty in UE3 and UE4
    ClassProperty,
    ArrayProperty,
    StructProperty,
    VectorProperty,
    RotatorProperty,
    StrProperty,
    MapProperty,
    FixedArrayProperty,	// NAME_InterfaceProperty in UE3
    DelegateProperty = 7,
    InterfaceProperty = 15,
};

static FName NAME_StringProperty = "StringProperty";
static FName NAME_ClassProperty = "ClassProperty";
static FName NAME_FixedArrayProperty = "FixedArrayProperty";

struct FLegacyPropertyTag
{
    FName		Name;
    FName		Type;				// FName in Unreal Engine; always equals to one of hardcoded type names ?
    FName		StructName;
    int32		ArrayIndex;
    int32		DataSize;
    int32		BoolValue;
    FName		EnumName;			// UE3 ver >= 633

    
	bool IsValid()
	{
		return Name != NAME_None;
	}

	friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive &Ar, FLegacyPropertyTag &Tag)
	{
	    static const struct
	    {
	        int32   	Index;
	        FName       Name;
	    } IndexToName[] = {
        { static_cast<int32>(EFLegacyPropType::ByteProperty), NAME_ByteProperty },
        { static_cast<int32>(EFLegacyPropType::IntProperty), NAME_IntProperty },
        { static_cast<int32>(EFLegacyPropType::BoolProperty), NAME_BoolProperty },
        { static_cast<int32>(EFLegacyPropType::FloatProperty), NAME_FloatProperty },
        { static_cast<int32>(EFLegacyPropType::ObjectProperty), NAME_ObjectProperty },
        { static_cast<int32>(EFLegacyPropType::NameProperty), NAME_NameProperty },
        { static_cast<int32>(EFLegacyPropType::StringProperty), NAME_StringProperty },
        { static_cast<int32>(EFLegacyPropType::ClassProperty), NAME_ClassProperty },
        { static_cast<int32>(EFLegacyPropType::ArrayProperty), NAME_ArrayProperty },
        { static_cast<int32>(EFLegacyPropType::StructProperty), NAME_StructProperty },
        { static_cast<int32>(EFLegacyPropType::VectorProperty), NAME_VectorProperty },
        { static_cast<int32>(EFLegacyPropType::RotatorProperty), NAME_RotatorProperty },
        { static_cast<int32>(EFLegacyPropType::StrProperty), NAME_StrProperty },
        { static_cast<int32>(EFLegacyPropType::MapProperty), NAME_MapProperty },
        { static_cast<int32>(EFLegacyPropType::FixedArrayProperty), NAME_FixedArrayProperty },
        { static_cast<int32>(EFLegacyPropType::DelegateProperty), NAME_DelegateProperty },
        { static_cast<int32>(EFLegacyPropType::InterfaceProperty), NAME_InterfaceProperty },
        };
	    
		Ar << Tag.Name;
		if (Tag.Name == NAME_None)
			return Ar;

		if (Ar.Game >= ERedUELegacyGame::UE3)
		{
			Ar << Tag.Type << Tag.DataSize << Tag.ArrayIndex;

		    bool bFound = false;
		    for(auto&[Index,Value]:IndexToName)
		    {
		        if(Value == Tag.Type)
		        {
		            bFound = true;
		            break;
		        }
		    }
		    checkf(bFound,TEXT("MapTypeName: unknown type '%s' for property '%s'"), *Tag.Type.ToString(), *Tag.Name.ToString());
			if (Tag.Type == NAME_StructProperty)
			{
			    Ar << Tag.StructName;
			}
			else if (Tag.Type == NAME_BoolProperty)
			{
				if (Ar.LegacyVer < 673)
					Ar << Tag.BoolValue;			// int
				else
					Ar << reinterpret_cast<uint8&>(Tag.BoolValue);		// byte
			}
			else if (Tag.Type == NAME_ByteProperty && Ar.LegacyVer >= 633)
			{
			    Ar << Tag.EnumName;
			}
			return Ar;
		}

		// UE1 and UE2
		uint8 info;
		Ar << info;

		bool IsArray = (info & 0x80) != 0;

	    bool bFound = false;
	    for(auto&[Index,Value]:IndexToName)
	    {
	        if(Index == static_cast<int32>(info & 0xF))
	        {
	            bFound = true;
	            Tag.Type = Value;
	            break;
	        }
	    }
	    checkf(bFound,TEXT("MapTypeName: unknown index type '%d' for property '%s'"), static_cast<int32>(info & 0xF), *Tag.Name.ToString());
	    
		if (Tag.Type == NAME_StructProperty)
			Ar << Tag.StructName;

		// analyze 'size' field
		Tag.DataSize = 0;
		switch ((info >> 4) & 7)
		{
		    case 0: Tag.DataSize = 1; break;
		    case 1: Tag.DataSize = 2; break;
		    case 2: Tag.DataSize = 4; break;
		    case 3: Tag.DataSize = 12; break;
		    case 4: Tag.DataSize = 16; break;
		    case 5: Ar << *reinterpret_cast<uint8*>(&Tag.DataSize); break;
		    case 6: Ar << *reinterpret_cast<uint16*>(&Tag.DataSize); break;
		    case 7: Ar << *((int32*)&Tag.DataSize); break;
		}

		Tag.ArrayIndex = 0;
		if (Tag.Type != NAME_BoolProperty && IsArray)	// 'bool' type has own meaning of 'array' flag
		{
			// read array index
			uint8 b;
			Ar << b;
			if (b < 128)
				Tag.ArrayIndex = b;
			else
			{
				uint8 b2;
				Ar << b2;
				if (b & 0x40)			// really, (b & 0xC0) == 0xC0
				{
					uint8 b3, b4;
					Ar << b3 << b4;
					Tag.ArrayIndex = ((b << 24) | (b2 << 16) | (b3 << 8) | b4) & 0x3FFFFF;
				}
				else
					Tag.ArrayIndex = ((b << 8) | b2) & 0x3FFF;
			}
		}
		Tag.BoolValue = 0;
		if (Tag.Type == NAME_BoolProperty)
			Tag.BoolValue = IsArray;

		return Ar;
	}
};

void ULegacyObject::PreLegacySerializeUnrealProps(FRedUELegacyArchive& Ar)
{
}

void ULegacyObject::LegacySerializeUnrealProps(UStruct* Type, void* Object, FRedUELegacyArchive& Ar)
{
	
	FLegacyPropertyTag LastTag;
    while (true)
    {
        FLegacyPropertyTag Tag;
        
        Ar << Tag;
        if (!Tag.IsValid())		
            break;
    	
    	LastTag = Tag;
        const int32 StopPos = Ar.Tell() + Tag.DataSize;

        FProperty* Property = Type->FindPropertyByName(Tag.Name);
        if(!Property||(!Property->HasAnyPropertyFlags(CPF_BlueprintVisible)||Property->HasAnyPropertyFlags(CPF_BlueprintReadOnly)))
        {
            Ar.Seek(StopPos);
            continue;
        }
        
        if (Tag.Type == NAME_ArrayProperty)
        {
            if (!ensure(Property->IsA(FArrayProperty::StaticClass()) && Tag.ArrayIndex == 0))
            {
                Ar.Seek(StopPos);
                continue;
            }
        }
        else if (!ensure(Tag.ArrayIndex == 0))
        {
            Ar.Seek(StopPos);
            continue;
        }

        auto TypeMismatch = [&]()
        {
            UE_LOG(LogRedUELegacy,Error, TEXT("Property %s expected type %s but read %s"), *Tag.Name.ToString(),  *Property->GetNameCPP(),*Tag.Type.ToString())
            Ar.Seek(StopPos);  
        };
        if(Tag.Type == NAME_StrProperty)
        {
            if(FStrProperty* StrProperty = CastField<FStrProperty>(Property))
            {
                FString InString;
                Ar << InString;
                StrProperty->SetValue_InContainer(Object,InString);
            }
            else if(FNameProperty* NameProperty = CastField<FNameProperty>(Property))
            {
                FString InString;
                Ar << InString;
                NameProperty->SetValue_InContainer(Object,*InString);
            }
            else
            {
                TypeMismatch();
            }
        }
    	else if(Tag.Type == NAME_NameProperty)
    	{
    		if(FNameProperty* NameProperty = CastField<FNameProperty>(Property))
    		{
    			FName InString;
    			Ar << InString;
    			NameProperty->SetValue_InContainer(Object,InString);
    		}
    		else
    		{
    			TypeMismatch();
    		}
    	}
        else  if(Tag.Type == NAME_BoolProperty)
        {
        	if(FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
        	{
        		BoolProperty->SetPropertyValue_InContainer(Object,Tag.BoolValue!=0);
        	}
        	else
        	{
        		TypeMismatch();
        	}
        }
        else  if(Tag.Type == NAME_IntProperty)
        {
        	if(FIntProperty* ObjectProperty = CastField<FIntProperty>(Property))
        	{
        		int32 Number;
        		Ar<<Number;
        		ObjectProperty->SetValue_InContainer(Object,Number);
        	}
        	else
        	{
        		TypeMismatch();
        	}
        }
        else  if(Tag.Type == NAME_FloatProperty)
        {
        	if(FFloatProperty* ObjectProperty = CastField<FFloatProperty>(Property))
        	{
        		float Number;
        		Ar<<Number;
        		ObjectProperty->SetValue_InContainer(Object,Number);
        	}
        	else
        	{
        		TypeMismatch();
        	}
        }
        else  if(Tag.Type == NAME_ObjectProperty)
        {
            if(FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
            {
                UObject*InObject;
                Ar<<InObject;
                ObjectProperty->SetObjectPropertyValue_InContainer(Object,InObject);
            	
            }
            else
            {
                TypeMismatch();
            }
        }
        else  if(Tag.Type == NAME_ByteProperty)
        {
        	if (Tag.DataSize != 1)
        	{
        		if(FByteProperty* ByteProperty = CastField<FByteProperty>(Property))
        		{
        			FName EnumValue;
        			Ar << EnumValue;
        			if(ensure(ByteProperty->Enum))
        			{
        				int32 EnumIndex =ByteProperty->Enum->GetValueByName(EnumValue);
        				if(ensure(EnumIndex != INDEX_NONE))
        				{
        					ByteProperty->SetValue_InContainer(Object,EnumIndex);
        				}
        			}
        		}
        		else if(FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
        		{
        			if(FIntProperty* IntProperty = CastField<FIntProperty>(EnumProperty->GetUnderlyingProperty()))
        			{
        				FName EnumValue;
        				Ar << EnumValue;
        				int32 EnumIndex = EnumProperty->GetEnum()->GetValueByName(EnumValue);
        				if(ensure(EnumIndex != INDEX_NONE))
        				{
        					void* PropAddr = EnumProperty->ContainerPtrToValuePtr<void>(Object);
        					IntProperty->SetIntPropertyValue(PropAddr,static_cast<int64>(EnumIndex));
        				}
        			}
					else
					{
						TypeMismatch();
					}
        		}
        		else
        		{
        			TypeMismatch();
        		}
        	}
        	else
        	{
        		if(FByteProperty* ByteProperty = CastField<FByteProperty>(Property))
        		{
        			uint8 Byte;
        			Ar<<Byte;
        			ByteProperty->SetValue_InContainer(Object,Byte);
        		}
				else
				{
					TypeMismatch();
				}
        	}
        }
        else  if(Tag.Type == NAME_StructProperty)
        {
            if(FStructProperty* StructProperty = CastField<FStructProperty>(Property))
            {
                if(StructProperty->Struct->UseNativeSerialization())
                {
                    Ar.Serialize( StructProperty->ContainerPtrToValuePtr<void>(Object),StructProperty->Struct->GetPropertiesSize());
                }
                else
                {
                    LegacySerializeUnrealProps(StructProperty->Struct,StructProperty->ContainerPtrToValuePtr<void>(Object),Ar);
                }
            }
            else
            {
                TypeMismatch();
            }
        }
        else if(Tag.Type == NAME_ArrayProperty)
        {
            if(FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
            {
                int32 DataCount;
                if ((Ar.Game&ERedUELegacyGame::Engine) == ERedUELegacyGame::UE2X && Ar.LegacyVer >= 145)
                {
                    Ar << DataCount;
                }
                else if (Ar.Game>=ERedUELegacyGame::UE3)
                {
                    Ar << DataCount;
                }
                else
                {
                    Ar << AR_INDEX(DataCount);
                }
               
                FScriptArrayHelper ArrayHelper(ArrayProperty,   Property->ContainerPtrToValuePtr<void>(Object));
                ArrayHelper.EmptyAndAddValues(DataCount);
                if(FObjectProperty* ObjectProperty = CastField<FObjectProperty>(ArrayProperty->Inner))
                {
                    for(int32 Index = 0;Index<DataCount;Index++)
                    {
                        UObject*InObject;
                    	Ar<<InObject;
						ObjectProperty->SetObjectPropertyValue(ArrayHelper.GetRawPtr(Index),InObject);
                    }
                }
            	else if(FStructProperty* StructProperty = CastField<FStructProperty>(ArrayProperty->Inner))
                {
            		for(int32 Index = 0;Index<DataCount;Index++)
            		{
            			if(StructProperty->Struct->UseNativeSerialization())
            			{
            				Ar.Serialize( ArrayHelper.GetRawPtr(Index),StructProperty->Struct->GetPropertiesSize());
            			}
            			else
            			{
            				LegacySerializeUnrealProps(StructProperty->Struct,ArrayHelper.GetRawPtr(Index),Ar);
            			}
            		}
                }
            	else if(FIntProperty* IntProperty = CastField<FIntProperty>(ArrayProperty->Inner))
            	{
            		for(int32 Index = 0;Index<DataCount;Index++)
            		{
            			int32 Number;
            			Ar<<Number;
            			IntProperty->SetValue_InContainer(ArrayHelper.GetRawPtr(Index),Number);
            		}
            	}
            	else if(FNameProperty* NameProperty = CastField<FNameProperty>(ArrayProperty->Inner))
            	{
            		for(int32 Index = 0;Index<DataCount;Index++)
            		{
            			FName InString;
            			Ar << InString;
            			NameProperty->SetValue_InContainer(ArrayHelper.GetRawPtr(Index),InString);
            		}
            	}
                else
                {
                    UE_LOG(LogRedUELegacy,Error, TEXT("PropertyArray %s has not support inner %s"), *Tag.Name.ToString(),*ArrayProperty->Inner->GetNameCPP());
                    ensureMsgf(false, TEXT("PropertyArray %s has not support inner %s"), *Tag.Name.ToString(),*ArrayProperty->Inner->GetNameCPP());
                }
                Ar.Seek(StopPos);
            }
            else
            {
                TypeMismatch();
            }
        }
        else
        {
            UE_LOG(LogRedUELegacy,Error, TEXT("Property %s has unimplemented type %s"), *Tag.Name.ToString(),*Tag.Type.ToString())
            ensureMsgf(false, TEXT("Property %s has unimplemented type %s"), *Tag.Name.ToString(),*Tag.Type.ToString());
            Ar.Seek(StopPos);
        }
       
    }
    //CastFieldChecked<FIntProperty>(Type->FindPropertyByName(NAME_Actor))->SetValue_InContainer();
    //LegacySerializeUnrealProps(CastFieldChecked<FStructProperty>( Type->FindPropertyByName(NAME_Actor))->Struct,   CastFieldChecked<FStructProperty>( Type->FindPropertyByName(NAME_Actor))->ContainerPtrToValuePtr<void>(Object),Ar);
}

UObject* ULegacyObject::ExportToContent()
{
    checkNoEntry();
    return nullptr;
}

FString ULegacyObject::GetLegacyFullName() const
{
    if(LegacyPackage)
    {
        return LegacyPackage->GetFullExportName(LegacyPackageIndex);
    }
    return TEXT("");
}

FString ULegacyObject::GetLegacyName() const
{
	if(LegacyPackage)
	{
		return LegacyPackage->GetExport(LegacyPackageIndex).ObjectName.ToString();
	}
	return TEXT("None");
}

FName ULegacyObject::GetLegacyFName() const
{
	if(LegacyPackage)
	{
		return LegacyPackage->GetExport(LegacyPackageIndex).ObjectName;
	}
	return NAME_None;
}


FString ULegacyObject::GetOutContentPath() const
{
	if(URedUELegacySubsystem* RedUELegacySubsystem = GetTypedOuter<URedUELegacySubsystem>())
	{
		return RedUELegacySubsystem->OutContentPath;
	}
	static const FString GamePath = TEXT("/Game");
	return GamePath;
}

FName ULegacyObject::GetLegacyClassName_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
    FString ClassName = GetClass()->GetName();
    if(ClassName.StartsWith(TEXT("Legacy")))
    {
        ClassName.RemoveAt(0,6);
    }
    return *ClassName;
}

bool ULegacyObject::LegacySupport_Implementation(ERedUELegacyEngineType EngineType, ERedUELegacyGameType GameType)
{
    return true;
}
