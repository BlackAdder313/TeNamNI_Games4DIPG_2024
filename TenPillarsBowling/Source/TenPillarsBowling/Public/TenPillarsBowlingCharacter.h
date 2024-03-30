// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TenPillarsBowlingCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATenPillarsBowlingCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Shoot Ball Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootBallAction;

	/** Shoot Ball Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CancelShootBallAction;

	/** Move Horizontally Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveHorizontallyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveBallTargetVerticallyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateAction;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	float HorizontalMovementRange = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	float VerticalTargetingRange = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	float RotationRange = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bowling|Constants")
	FVector CameraOffset = FVector(-700.f, 0.f, 200.f);
	
public:
	ATenPillarsBowlingCharacter();

	virtual void BeginPlay() override;

protected:
	void StartShootBall(const FInputActionValue& Value);
	void ExecuteShootBall(const FInputActionValue& Value);
	void CancelShootBall(const FInputActionValue& Value);

	/** Called for movement input */
	void MoveHorizontally(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);

	void MoveBallTargetVertically(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

