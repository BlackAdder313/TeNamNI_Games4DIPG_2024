// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_Pin.h"
#include "TenPillarsBowlingCharacter.h"
#include "TenPillarsBowlingProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
ATP_Pin::ATP_Pin()
{
	if (UPrimitiveComponent* meshComponent = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		meshComponent->SetSimulatePhysics(true);
		// Probably need to remember to add generate overlap events on the terrain / landscape
	}

	OnActorBeginOverlap.AddUniqueDynamic(this, &ATP_Pin::OnBeginOverlap);
}


void ATP_Pin::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ATP_Pin>(OtherActor))
	{
		// We don't care if it overlaps with another pin
		return;
	}
	
	// If it overlapped something, and it's falling down,
	// well, then it fall down
	if (GetActorRotation().Yaw > 80.f)
	{
		OnPinDropped.Broadcast(PinNumber);
	}
}

void ATP_Pin::SetupPin(int32 pinNumber, FVector position, FRotator location)
{
}