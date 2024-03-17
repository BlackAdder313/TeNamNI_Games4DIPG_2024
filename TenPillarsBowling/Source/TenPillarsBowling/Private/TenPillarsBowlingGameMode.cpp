// Copyright Epic Games, Inc. All Rights Reserved.

#include "TenPillarsBowlingGameMode.h"
#include "TenPillarsBowlingCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATenPillarsBowlingGameMode::ATenPillarsBowlingGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
