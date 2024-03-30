// Copyright Epic Games, Inc. All Rights Reserved.

#include "TenPillarsBowlingCharacter.h"
#include "TenPillarsBowlingPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATenPillarsBowlingCharacter

ATenPillarsBowlingCharacter::ATenPillarsBowlingCharacter()
{	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

void ATenPillarsBowlingCharacter::BeginPlay()
{
	Super::BeginPlay();
	FirstPersonCameraComponent->SetRelativeLocation(CameraOffset); // Position the camera

	isShootBallCancelled = false;
}

//////////////////////////////////////////////////////////////////////////// Input

void ATenPillarsBowlingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Ball Shooting
		EnhancedInputComponent->BindAction(ShootBallAction, ETriggerEvent::Triggered, this, &ATenPillarsBowlingCharacter::StartShootBall);
		EnhancedInputComponent->BindAction(ShootBallAction, ETriggerEvent::Completed, this, &ATenPillarsBowlingCharacter::ExecuteShootBall);
		EnhancedInputComponent->BindAction(CancelShootBallAction, ETriggerEvent::Started, this, &ATenPillarsBowlingCharacter::CancelShootBall);

		// Moving
		EnhancedInputComponent->BindAction(MoveHorizontallyAction, ETriggerEvent::Triggered, this, &ATenPillarsBowlingCharacter::MoveHorizontally);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ATenPillarsBowlingCharacter::Rotate);

		// Looking
		EnhancedInputComponent->BindAction(MoveBallTargetVerticallyAction,
										   ETriggerEvent::Triggered,
										   this,
										   &ATenPillarsBowlingCharacter::MoveBallTargetVertically);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ATenPillarsBowlingCharacter::MoveHorizontally(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATenPillarsBowlingCharacter::Rotate(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		CastChecked<ATenPillarsBowlingPlayerController>(Controller)->OnPlayerRotate(0.f, LookAxisVector.X);
	}
}

void ATenPillarsBowlingCharacter::StartShootBall(const FInputActionValue& Value)
{
	if (Controller != nullptr && !isShootBallCancelled)
	{
		CastChecked<ATenPillarsBowlingPlayerController>(Controller)->UpdateBallShootPower();
	}
}

void ATenPillarsBowlingCharacter::ExecuteShootBall(const FInputActionValue& Value)
{
	isShootBallCancelled = false;
	if (Controller != nullptr)
	{
		CastChecked<ATenPillarsBowlingPlayerController>(Controller)->OnBallShoot();
	}
}

void ATenPillarsBowlingCharacter::CancelShootBall(const FInputActionValue& Value)
{
	isShootBallCancelled = true;
	if (Controller != nullptr)
	{
		CastChecked<ATenPillarsBowlingPlayerController>(Controller)->ResetBallShootPower();
	}
}

void ATenPillarsBowlingCharacter::MoveBallTargetVertically(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		CastChecked<ATenPillarsBowlingPlayerController>(Controller)->OnPlayerRotate(LookAxisVector.X, 0.f);
	}
}