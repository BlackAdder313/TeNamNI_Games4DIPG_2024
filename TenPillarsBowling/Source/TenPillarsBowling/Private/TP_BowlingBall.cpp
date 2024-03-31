// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_BowlingBall.h"


ATP_BowlingBall::ATP_BowlingBall()
{
	GetStaticMeshComponent()->Mobility = EComponentMobility::Movable;
	GetStaticMeshComponent()->SetSimulatePhysics(false);
	GetStaticMeshComponent()->SetGenerateOverlapEvents(false);
	GetStaticMeshComponent()->bUseDefaultCollision = true;
}

void ATP_BowlingBall::SetupBall(const FVector& initialPosition)
{
	GetStaticMeshComponent()->SetSimulatePhysics(false);
	SetActorRelativeLocation(initialPosition);
	BP_OnBallSetup();
}

void ATP_BowlingBall::OnShootExecuted()
{
	GetStaticMeshComponent()->SetSimulatePhysics(true);
	BP_OnShootExecuted();
}
