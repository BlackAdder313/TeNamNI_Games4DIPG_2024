// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "TP_Pin.generated.h"

class ATenPillarsBowlingCharacter;

UCLASS()
class TENPILLARSBOWLING_API ATP_Pin : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ATP_Pin();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPinDropped, int32, pinIndex);

	UFUNCTION(BlueprintGetter)
	int32 GetPinNumber() { return PinNumber; }

	void SetupPin(int32 pinNumber, FVector position, FRotator rotation);
	void PrepareForNextFrameStage();
	void OnShootExecuted();

	FOnPinDropped OnPinDropped;

protected:
	UPROPERTY(BlueprintGetter = GetPinNumber, Category = "Pin Properties", BlueprintReadOnly, VisibleDefaultsOnly)
	int32 PinNumber;

	UPROPERTY(Category = "Pin Properties", BlueprintReadOnly, VisibleDefaultsOnly)
	FVector PinPosition;

	UPROPERTY(Category = "Pin Properties", BlueprintReadOnly, VisibleDefaultsOnly)
	FRotator PinRotation;

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	/** The Character holding this weapon*/
	ATenPillarsBowlingCharacter* Character;

	bool beDestroyed = false;
};
