#include "Kismet/Engine/SeqEvent_Console.h"

void USeqEvent_Console::In()
{
	Out.Broadcast();
}
