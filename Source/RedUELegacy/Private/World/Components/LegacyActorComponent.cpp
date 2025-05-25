// Tyran


#include "World/Components/LegacyActorComponent.h"


void ULegacyActorComponent::LegacySerializeComponent(FArchive& Ar)
{
    auto IsTemplate = [this]( int64 TemplateTypes = (0x400|0x200) ) 
    {
        for ( const ULegacyObject* TestOuter = this; TestOuter; TestOuter = Cast<ULegacyObject>( TestOuter->GetOuter()) )
        {
            if ( !!(TestOuter->LegacyObjectFlags & TemplateTypes) )
            {
                return true;
            }
        }
        return false;
    };
    
    Super::LegacySerializeComponent(Ar);
    UObject* TemplateOwnerClass;
    Ar<<TemplateOwnerClass;

    
    if (IsTemplate(0x200))
    {
        FName TemplateName;
        Ar<<TemplateName;
    }
}

void ULegacyActorComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
}
