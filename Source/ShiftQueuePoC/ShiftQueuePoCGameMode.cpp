// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ShiftQueuePoC.h"
#include "ShiftQueuePoCGameMode.h"
#include "ShiftQueuePoCPlayerController.h"
#include "ShiftQueuePoCCharacter.h"

AShiftQueuePoCGameMode::AShiftQueuePoCGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AShiftQueuePoCPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}