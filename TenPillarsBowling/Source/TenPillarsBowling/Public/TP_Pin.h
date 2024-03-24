// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_Pin.generated.h"

class ATenPillarsBowlingCharacter;

UCLASS()
class TENPILLARSBOWLING_API ATP_Pin : public AActor
{
	GENERATED_BODY()

public:
	ATP_Pin();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPinDropped, int32, pinIndex);

	void SetupPin(int32 pinNumber, FVector position, FRotator location);

	UFUNCTION(BlueprintGetter)
	int32 GetPinNumber() { return PinNumber; }

	FOnPinDropped OnPinDropped;

protected:
	UPROPERTY(BlueprintGetter = GetPinNumber, Category = "Pin Properties", BlueprintReadOnly, VisibleDefaultsOnly)
	int32 PinNumber;

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	/** The Character holding this weapon*/
	ATenPillarsBowlingCharacter* Character;
};
