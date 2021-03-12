// Copyright Epic Games, Inc. All Rights Reserved.

#include "DroneMadnessGameMode.h"
#include "DroneMadnessPawn.h"

ADroneMadnessGameMode::ADroneMadnessGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ADroneMadnessPawn::StaticClass();
}

