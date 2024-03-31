// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Map.h"
#include "TenPillarsBowlingUI.generated.h"

/**
 *  Simple Vehicle HUD class
 *  Displays the current speed and gear.
 *  Widget setup is handled in a Blueprint subclass.
 */
UCLASS(abstract)
class TENPILLARSBOWLING_API UTenPillarsBowlingUI : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Called to update the speed display */
	void UpdateScore(int32 frameNumber, const TArray<int32>& scoreBoard);

	/** Called to update the planted wheat display */
	void UpdateForce(int32 force);

	/** Called to update the planted wheat display */
	void ResetForce();

protected:

	/** Implemented in Blueprint to display the new speed */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnScoreUpdated(int32 frameNumber, const TArray<int32>& score);

	/** Implemented in Blueprint to display the new speed */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnForceUpdated(int32 force);

	/** Implemented in Blueprint to display the new speed */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnForceReset();
};
