#include "XSeqAct_PatternMatcherSetGlobalFlag.h"

void UXSeqAct_PatternMatcherSetGlobalFlag::Set()
{
	Out.Broadcast();
}

void UXSeqAct_PatternMatcherSetGlobalFlag::Unset()
{
	Out.Broadcast();
}

void UXSeqAct_PatternMatcherSetGlobalFlag::Toggle()
{
	Out.Broadcast();
}
