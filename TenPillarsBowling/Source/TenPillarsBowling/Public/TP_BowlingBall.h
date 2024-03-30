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

private:
	FVector m_initialPosition;
};
