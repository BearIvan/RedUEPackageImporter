#include "RedUEBlueprintFunctionLibrary.h"

#include "Engine/LevelScriptActor.h"


void URedUEBlueprintFunctionLibrary::RemoteEventFromLevelScript(FName EventName, UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for( TArray<ULevel*>::TConstIterator it = World->GetLevels().CreateConstIterator(); it; ++it )
		{
			ULevel* CurLevel = *it;
			if( CurLevel && CurLevel->bIsVisible )
			{
				ALevelScriptActor* LSA = CurLevel->GetLevelScriptActor();
				if( LSA )
				{
					// Find an event with no parameters
					UFunction* EventTarget = LSA->FindFunction(EventName);
					if( EventTarget && EventTarget->NumParms == 0)
					{
						LSA->ProcessEvent(EventTarget, NULL);
					}
				}
			}
		}
	}
}
