// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "TP_BowlingBall.generated.h"

class ATenPillarsBowlingCharacter;

UCLASS()
class TENPILLARSBOWLING_API ATP_BowlingBall : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ATP_BowlingBall();

	void SetupBall(const FVector& initialPosition);
	void OnShootExecuted();

protected:
	/** Implemented in Blueprint to display the new speed */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void BP_OnBallSetup();

	/** Implemented in Blueprint to display the new speed */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void BP_OnShootExecuted();

private:
	FVector m_initialPosition;
};
