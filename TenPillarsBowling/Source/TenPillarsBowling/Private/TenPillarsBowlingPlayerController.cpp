// Copyright Epic Games, Inc. All Rights Reserved.


#include "TenPillarsBowlingPlayerController.h"
#include "Engine/StaticMeshActor.h"
#include "EnhancedInputSubsystems.h"

#include "TenPillarsBowlingUI.h"
#include "TP_Pin.h"
#include "TP_BowlingBall.h"

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

	// spawn the UI widget and add it to the viewport
	PlayerUI = CreateWidget<UTenPillarsBowlingUI>(this, PlayerUIClass);

	check(PlayerUI);

	PlayerUI->AddToViewport();

	PrepareGame();
}

void ATenPillarsBowlingPlayerController::Tick(float deltaSeconds)
{
	if(!m_isTimerRunning)
	{
		return;
	}

	m_roundTimer += deltaSeconds;
	if (m_roundTimer >= 10.f)
	{
		EvaluateFrame();
	}
}

void ATenPillarsBowlingPlayerController::PrepareGame()
{
	CurrentFrame = -1; // meh
	numberOfDroppedPinsOnShot = 0;
	
	remainingExtraShots.Reset();

	scoringWaitingList.Empty();
	pointsPerFrame.Init(0, NumberOfRounds);

	PrepareFrame();

	m_roundTimer = 0.f;
}

void ATenPillarsBowlingPlayerController::SetupBowlingBall()
{
	if (bowlingBall)
	{
		bowlingBall->Destroy();
	}

	bowlingBall = GetWorld()->SpawnActor<AStaticMeshActor>(BowlingBallClass, FVector() , FRotator::ZeroRotator);
	bowlingBall->AttachToActor(GetPawn(), FAttachmentTransformRules::KeepWorldTransform);
	CastChecked<ATP_BowlingBall>(bowlingBall)->SetupBall(FVector(BowlingBallDistanceFromPlayer, 0.f, 50.f /* small height offset*/));
	ResetBallShootPower();
}

void ATenPillarsBowlingPlayerController::UpdateBallShootPower()
{
	if (FrameState != EFrameState::Start)
	{
		return;
	}

	shotPower += (shotPowerDirection * ShotPowerChangeStep);
	if (shotPower > 100 || shotPower < 0)
	{
		shotPowerDirection *= -1;
	}

	shotPower = FMath::Clamp(shotPower, 0, 100);

	if (PlayerUI)
	{
		PlayerUI->UpdateForce(shotPower);
	}
}

void ATenPillarsBowlingPlayerController::ResetBallShootPower()
{
	shotPower = 0;
	shotPowerDirection = 1;
}

void ATenPillarsBowlingPlayerController::SetupPins()
{
	if (!EnablePinSpawning)
	{
		return;
	}
	
	for (auto pin : pins)
	{
		pin->Destroy();
	}
	pins.Empty();
	
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
				pin->SetupPin(pinIndex, position, FRotator::ZeroRotator);
				pinIndex++;

				pin->OnPinDropped.AddUniqueDynamic(this, &ATenPillarsBowlingPlayerController::OnPinDropped);
				pins.Add(pin);
			}
		}
	}
}

void ATenPillarsBowlingPlayerController::PrepareFrame()
{
	for (int index = 0; index < pointsPerFrame.Num(); index++)
	{
		UE_LOG(LogPlayerController, Error, TEXT("Frame: %d -- Points: %d"), index, pointsPerFrame[index]);
	}

	if (CurrentFrame == NumberOfRounds - 1)
	{
		if (numberOfDroppedPinsOnFrame < 10 || remainingExtraShots.IsSet() && !remainingExtraShots.GetValue())
		{
			// Finish level
			UE_LOG(LogPlayerController, Error, TEXT("Game over"));

			int pinsAmount = 0;
			for (int index = 0; index < pointsPerFrame.Num(); index++)
			{
				pinsAmount += pointsPerFrame[index];
				UE_LOG(LogPlayerController, Error, TEXT("Frame: %d -- Total: %d"), index, pinsAmount);
			}

			return;
		}

		if (remainingExtraShots.IsSet())
		{
			remainingExtraShots = remainingExtraShots.GetValue() - 1;
		}
		else
		{
			// Get 2 extra attempts if last shot of normal rounds was a strike,
			// else 1 if it was a spare
			remainingExtraShots = FrameStage == 1 ? 2 : 1;
		}
	}
	
	CurrentFrame = FMath::Min(CurrentFrame + 1, NumberOfRounds - 1);
	numberOfDroppedPinsOnFrame = 0;
	FrameState = EFrameState::Start;
	FrameStage = 1;

	SetupBowlingBall();
	SetupPins();

	m_roundTimer = 0.f;

	if (PlayerUI)
	{
		PlayerUI->ResetForce();
	}
}

void ATenPillarsBowlingPlayerController::EvaluateFrame()
{
	m_isTimerRunning = false;
	FrameState = EFrameState::ScoreCalculation;
	
	shotsPerFrame.FindOrAdd(CurrentFrame + 1).Add(numberOfDroppedPinsOnShot);
	numberOfDroppedPinsOnShot = 0;

	for (int32 index = scoringWaitingList.Num() - 1; index >= 0; index--)
	{
		auto& waitingFrame = scoringWaitingList[index];
		pointsPerFrame[waitingFrame.X] += numberOfDroppedPinsOnFrame;
		
		waitingFrame.Y--;		
		if (!waitingFrame.Y)
		{
			scoringWaitingList.RemoveAt(index);
		}
	}

	bool isSpare = numberOfDroppedPinsOnFrame == 10;
	bool isStrike = isSpare && FrameStage == 1;
	if (FrameStage == 2 || isStrike)
	{
		pointsPerFrame[CurrentFrame] += numberOfDroppedPinsOnFrame;

		if(CurrentFrame < NumberOfRounds - 1)
		{
			if (isStrike)
			{
				scoringWaitingList.Push({ CurrentFrame , 2 });
			}
			else if (isSpare)
			{
				scoringWaitingList.Push({ CurrentFrame , 1 });
			}
		}

		if (PlayerUI)
		{
			PlayerUI->UpdateScore(CurrentFrame + 1, shotsPerFrame[CurrentFrame + 1]);
		}

		PrepareFrame();
		return;
	}

	numberOfDroppedPinsOnFrame = 0; // adapt to remove for tests?
	FrameStage++;
	for (auto pin : pins)
	{
		CastChecked<ATP_Pin>(pin)->PrepareForNextFrameStage();
	}

	m_roundTimer = 0.f;
	FrameState = EFrameState::Start;
	SetupBowlingBall();
	
	if (PlayerUI)
	{
		PlayerUI->UpdateScore(CurrentFrame + 1, shotsPerFrame[CurrentFrame + 1]);
	}
}

void ATenPillarsBowlingPlayerController::OnPinDropped(int32 pinIndex)
{
	UE_LOG(LogPlayerController, Error, TEXT("Pin dropped with index: %d"), pinIndex);
	numberOfDroppedPinsOnFrame++;
	numberOfDroppedPinsOnShot++;
	if (numberOfDroppedPinsOnFrame == 10)
	{
		EvaluateFrame();
	}
}

void ATenPillarsBowlingPlayerController::OnPlayerRotate(float verticalOffset, float horizontalOffset)
{
	auto castBowlingBall = CastChecked<ATP_BowlingBall>(bowlingBall);
	FRotator rotation = castBowlingBall->GetActorRotation();
	castBowlingBall->SetActorRotation(rotation + FRotator(verticalOffset, horizontalOffset, 0.f));
}

void ATenPillarsBowlingPlayerController::OnBallShoot()
{
	if (!shotPower || FrameState != EFrameState::Start)
	{
		return;
	}

	FrameState = EFrameState::BallRolling;

	for (auto pin : pins)
	{
		CastChecked<ATP_Pin>(pin)->OnShootExecuted();
	}
	
	CastChecked<ATP_BowlingBall>(bowlingBall)->OnShootExecuted();

	float totalBallPower = MaxShotPower * shotPower;
	bowlingBall->GetStaticMeshComponent()->AddImpulse(totalBallPower * bowlingBall->GetActorForwardVector(), NAME_None, true);
	
	m_isTimerRunning = true;
}