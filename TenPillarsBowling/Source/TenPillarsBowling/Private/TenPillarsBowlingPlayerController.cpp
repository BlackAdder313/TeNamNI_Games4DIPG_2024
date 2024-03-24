// Copyright Epic Games, Inc. All Rights Reserved.


#include "TenPillarsBowlingPlayerController.h"
#include "EnhancedInputSubsystems.h"

#include "TP_Pin.h"

static const TArray<TArray<float>> s_PinsLayout =
{
	{0.f},
	{-.5f, .5f},
	{-1.f, 0.f, 1.f},
	{-1.5f, -0.5f, .5f, 1.5f}
};

void ATenPillarsBowlingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	PrepareGame();
}

void ATenPillarsBowlingPlayerController::PrepareGame()
{
	CurrentFrame = 1;

	scoringWaitingList.Init(0, NumberOfRounds);
	pointsPerFrame.Init(0, NumberOfRounds);

	SetupPins();
	PrepareFrame();
}

void ATenPillarsBowlingPlayerController::SetupPins()
{
	if (!EnablePinSpawning)
	{
		return;
	}
	int pinIndex = 1;
	for (int8 index = 0; index < s_PinsLayout.Num(); index++)
	{
		const auto& pinsRowOffsets = s_PinsLayout[index];
		for (int8 offsetIndex = 0; offsetIndex < pinsRowOffsets.Num(); offsetIndex++)
		{
			FVector position = FVector(FirstPinCoordinates.X + PinVerticalOffset * index,
									   FirstPinCoordinates.Y + PinHorizontalOffset * pinsRowOffsets[offsetIndex],
									   0.f);
			auto pinActor = GetWorld()->SpawnActor<AActor>(PinClass, position, FRotator());
			if (ATP_Pin* pin = Cast<ATP_Pin>(pinActor))
			{
				pin->SetupPin(pinIndex, position, FRotator());
				pinIndex++;

				pin->OnPinDropped.AddUniqueDynamic(this, &ATenPillarsBowlingPlayerController::OnPinDropped);
				pins.Add(pin);
			}
		}
	}
}

void ATenPillarsBowlingPlayerController::PrepareFrame()
{
	numberOfDroppedPinsOnFrame = 0;
	FrameState = EFrameState::Start;
	FrameStage = 1;
}

void ATenPillarsBowlingPlayerController::EvaluateFrame()
{
	bool isStrike = numberOfDroppedPinsOnFrame == 10 && FrameStage == 1;
	if (FrameStage == 2 || isStrike)
	{
		pointsPerFrame[CurrentFrame] = numberOfDroppedPinsOnFrame;
		if (isStrike)
		{
			scoringWaitingList[CurrentFrame] = 2;
		}
		else if (numberOfDroppedPinsOnFrame == 10) // Spare
		{
			scoringWaitingList[CurrentFrame] = 1;
		}
		// Calculate points
		// If it's spare of strike, add it to 
	}
}

void ATenPillarsBowlingPlayerController::OnPinDropped(int32 pinIndex)
{
	numberOfDroppedPinsOnFrame++;
	if (numberOfDroppedPinsOnFrame == 10)
	{
		// TODO: Pause timer
		EvaluateFrame();
	}
}