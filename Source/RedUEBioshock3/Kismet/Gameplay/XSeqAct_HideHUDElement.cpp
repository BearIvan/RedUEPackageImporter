#include "XSeqAct_HideHUDElement.h"

void UXSeqAct_HideHUDElement::HideElements()
{
	Out.Broadcast();
}

void UXSeqAct_HideHUDElement::ShowElements()
{
	Out.Broadcast();
}
