#include "XSeqAct_InstancePattern.h"

#include "Components/AudioComponent.h"
#include "Interfaces/Bioshock/Pawn/RedXPawnInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/Base/LegacyKismet.h"

UXPatternCommandBase::UXPatternCommandBase()
{
}

void UXPatternCommandBase::Begin()
{
}

bool UXPatternCommandBase::CanJumpNextCommand()
{
	return true;
}

void UXPatternCommandBase::Tick(float DeltaTime)
{
}

void UXPatternCommandBase::End()
{
}

UXPatternObjectSetBase::UXPatternObjectSetBase()
{
}

APawn* UXPatternObjectSetBase::GetPawn()
{
	return nullptr;
}

UXPatternObjectSet_Player::UXPatternObjectSet_Player()
{
}

APawn* UXPatternObjectSet_Player::GetPawn()
{
	if (ALegacyKismet* Kismet = GetTypedOuter<ALegacyKismet>())
	{
		if ( Kismet->PlayerController)
		{
			return Kismet->PlayerController->GetPawn();
		}
	}
	return nullptr;
}

UXPatternObjectSet_ElizabethPawn::UXPatternObjectSet_ElizabethPawn()
{
}

APawn* UXPatternObjectSet_ElizabethPawn::GetPawn()
{
	if (ABioshockKismet* Kismet = GetTypedOuter<ABioshockKismet>())
	{
		if ( Kismet->ElizabethPawn)
		{
			return Kismet->ElizabethPawn;
		}
	}
	return nullptr;
}

UXPatternCommand_KismetEvent::UXPatternCommand_KismetEvent()
{
	
}

void UXPatternCommand_KismetEvent::Begin()
{
	Super::Begin();
	
	if (ABioshockKismet* Kismet = GetTypedOuter<ABioshockKismet>())
	{
		if (UFunction* Function = Kismet->FindFunction(EventName))
		{
			Kismet->ProcessEvent(Function,nullptr);
		}
	}
}

UXPatternCommand_Speak::UXPatternCommand_Speak(): Audio(nullptr)
{
}

void UXPatternCommand_Speak::Begin()
{
	Super::Begin();
	if (UXSeqAct_InstancePattern* InstancePattern = GetTypedOuter<UXSeqAct_InstancePattern>())
	{
		InstancePattern->LastPawn = Speaker->GetPawn();
		if (InstancePattern->LastPawn && InstancePattern->LastPawn->GetClass()->ImplementsInterface(URedXPawnInterface::StaticClass()))
		{
			IRedXPawnInterface::Execute_Speak(InstancePattern->LastPawn,Audio);
		}
	}
}

UXPatternCommand_PlayAudio::UXPatternCommand_PlayAudio(): Audio(nullptr)
{
}

void UXPatternCommand_PlayAudio::Begin()
{
	Super::Begin();
	
	if (UXSeqAct_InstancePattern* InstancePattern = GetTypedOuter<UXSeqAct_InstancePattern>())
	{
		InstancePattern->LastAudio = UGameplayStatics::SpawnSound2D(GetWorld(),Audio);
	}
}

UXPatternCommand_AudioTimeElapsed::UXPatternCommand_AudioTimeElapsed()
{
}

void UXPatternCommand_AudioTimeElapsed::Begin()
{
	Super::Begin();
}

void UXPatternCommand_AudioTimeElapsed::Tick(float Time)
{
	Super::Tick(Time);
}

bool UXPatternCommand_AudioTimeElapsed::CanJumpNextCommand()
{
	if (UXSeqAct_InstancePattern* InstancePattern = GetTypedOuter<UXSeqAct_InstancePattern>())
	{
		if (InstancePattern->LastPawn && InstancePattern->LastPawn->GetClass()->ImplementsInterface(URedXPawnInterface::StaticClass()))
		{
			if (IRedXPawnInterface::Execute_IsSpeaking(InstancePattern->LastPawn))
			{
				return false;
			}
		}
	}
	return Super::CanJumpNextCommand();
}

UXPatternCommand_TimeElapsed::UXPatternCommand_TimeElapsed()
{
}

void UXPatternCommand_TimeElapsed::Begin()
{
	Super::Begin();
	CurrentTime = SleepDurationSeconds;
}

void UXPatternCommand_TimeElapsed::Tick(float Time)
{
	Super::Tick(Time);
	CurrentTime -= Time;
}

bool UXPatternCommand_TimeElapsed::CanJumpNextCommand()
{
	if (bUseAudioTime)
	{
		if (UXSeqAct_InstancePattern* InstancePattern = GetTypedOuter<UXSeqAct_InstancePattern>())
		{
			if (IsValid(InstancePattern->LastAudio))
			{
				if (InstancePattern->LastAudio->IsPlaying())
				{
					return false;
				}
			}
		}
	}
	return CurrentTime <= 0;
}

void UXSeqAct_InstancePattern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentCommands <= Commands.Num() && CurrentCommands >= 0)
	{
		if (CurrentCommands == 0)
		{
			if (Commands.Num() > 0)
			{
				Commands[CurrentCommands]->Begin();
				CurrentCommands++;
				return;
			}
			else
			{
				CurrentCommands = -1;
				return;
			}
		}
	
		if (Commands[CurrentCommands-1]->CanJumpNextCommand())
		{
			Commands[CurrentCommands-1]->End();
			if (CurrentCommands < Commands.Num())
			{
				Commands[CurrentCommands]->Begin();
			}
			CurrentCommands++;
		}
		else
		{
			Commands[CurrentCommands-1]->Tick(DeltaTime);
		}
	}
	else
	{
		LastPawn = nullptr;
	}
}

void UXSeqAct_InstancePattern::In()
{
	CurrentCommands = 0;
}

void UXSeqAct_InstancePattern::DestroyInstance()
{
}
