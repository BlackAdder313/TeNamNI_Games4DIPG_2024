// Copyright Epic Games, Inc. All Rights Reserved.


#include "TenPillarsBowlingUI.h"

void UTenPillarsBowlingUI::UpdateScore(int32 frameNumber, const TArray<int32>& scoreBoard)
{
	// call the Blueprint handler
	OnScoreUpdated(frameNumber, scoreBoard);
}

void UTenPillarsBowlingUI::UpdateForce(int32 force)
{
	// call the Blueprint handler
	OnForceUpdated(force);
}

void UTenPillarsBowlingUI::ResetForce()
{
	// call the Blueprint handler
	OnForceReset();
}