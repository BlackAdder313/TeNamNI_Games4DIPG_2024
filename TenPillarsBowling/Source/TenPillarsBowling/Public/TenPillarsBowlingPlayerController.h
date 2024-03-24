// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TenPillarsBowlingPlayerController.generated.h"

class UInputMappingContext;

UENUM()
enum class EFrameState : uint8
{
	Start,
	BallRolling,
	ScoreCalculation,
	Finish
};

UCLASS()
class TENPILLARSBOWLING_API ATenPillarsBowlingPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AActor> PinClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	bool EnablePinSpawning = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	int32 NumberOfRounds = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	float PinVerticalOffset = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	float PinHorizontalOffset = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	FVector2f FirstPinCoordinates = { 4520.f, 1690.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	int32 RoundWaitTimeInSeconds = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bowling|GameDetails")
	int32 CurrentFrame = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bowling|GameDetails|Frame")
	int32 FrameStage = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bowling|GameDetails|Frame")
	EFrameState FrameState = EFrameState::Start;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPinDropped(int32 pinIndex);

private:
	void PrepareGame();

	void SetupPins();
	TArray<AActor*> pins;
	
	// Frame Properties
	void PrepareFrame();
	void EvaluateFrame();
	int32 numberOfDroppedPinsOnFrame = 0;
	
	// Scoring
	
	// Length of array: RoundsNumber
	// Index of element: Round number
	// Value of element: Number of rounds to retroactively add their scores to this round
	TArray<int> scoringWaitingList;

	TArray<int> pointsPerFrame;
};
