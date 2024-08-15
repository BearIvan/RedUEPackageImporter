
#include "Blueprints/RedUEBlueprintFunctionLibrary.h"

class FRedUEToggleCinematicModeAction : public FPendingLatentAction
{
public:
	FRedUEToggleCinematicModeAction(const FLatentActionInfo& InLatentInfo,ERedUEToggleCinematicModeThen& InThen,const FRedUEToggleCinematicModeParameters& InParameters):LatentInfo(InLatentInfo),Then(InThen),Parameters(InParameters)
	{
		
	}
	
	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Then = ERedUEToggleCinematicModeThen::Out;
		Response.FinishAndTriggerIf(true,LatentInfo.ExecutionFunction,LatentInfo.Linkage,LatentInfo.CallbackTarget);
	}
	ERedUEToggleCinematicModeThen& Then;
	FLatentActionInfo LatentInfo;
	FRedUEToggleCinematicModeParameters Parameters;
};
void URedUEBlueprintFunctionLibrary::ToggleCinematicMode(ERedUEToggleCinematicModeExec Exec, ERedUEToggleCinematicModeThen& Then, UObject* WorldContextObject, FLatentActionInfo LatentInfo, const FRedUEToggleCinematicModeParameters& Parameters)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentManager = World->GetLatentActionManager();
		if (LatentManager.FindExistingAction<FStreamLevelAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			FRedUEToggleCinematicModeAction* NewAction = new FRedUEToggleCinematicModeAction(LatentInfo,Then,Parameters);
			LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction);
		}
	}
}
