// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_Pin.h"
#include "TP_BowlingBall.h"

// Sets default values for this component's properties
ATP_Pin::ATP_Pin()
{
	GetStaticMeshComponent()->Mobility = EComponentMobility::Movable;
	GetStaticMeshComponent()->SetGenerateOverlapEvents(false);
	GetStaticMeshComponent()->bUseDefaultCollision = true;

	OnActorBeginOverlap.AddUniqueDynamic(this, &ATP_Pin::OnBeginOverlap);
}


void ATP_Pin::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

	if (!OtherActor->ActorHasTag("Floor"))
	{
		// We don't care if it overlaps with another pin or the ball
		return;
	}
	
	OnPinDropped.Broadcast(PinNumber);
	OnActorBeginOverlap.RemoveAll(this);
	beDestroyed = true;
}

void ATP_Pin::SetupPin(int32 pinNumber, FVector position, FRotator rotation)
{
	PinNumber = pinNumber;
	PinPosition = position;
	PinRotation = rotation;
	GetStaticMeshComponent()->SetSimulatePhysics(false);
}

void ATP_Pin::OnShootExecuted()
{
	GetStaticMeshComponent()->SetSimulatePhysics(true);
}

void ATP_Pin::PrepareForNextFrameStage()
{
	if (beDestroyed || IsPendingKillPending())
	{
		SetActorHiddenInGame(true);		
		return;
	}

	GetStaticMeshComponent()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	GetStaticMeshComponent()->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetStaticMeshComponent()->SetSimulatePhysics(false);
	SetActorLocationAndRotation(PinPosition, PinRotation);
}