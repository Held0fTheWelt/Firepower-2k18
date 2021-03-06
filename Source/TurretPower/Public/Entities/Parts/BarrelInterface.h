// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BarrelInterface.generated.h"

// Enum for TankAimingComponent State
UENUM(Meta = (Bitflags))
enum class EBarrelType : uint8
{
	None = 4,
	Cannon = 0,
	Gun = 1,
	Rocket = 2
};

UINTERFACE(Blueprintable)
class UBarrelInterface : public UInterface
{
	GENERATED_BODY()
};


class TURRETPOWER_API IBarrelInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	virtual EBarrelType GetBarrelType() const;

	UFUNCTION()
	virtual void Elevate(float RelativeSpeed);

	UFUNCTION()
	virtual FVector GetMyForwardVector() const;

	virtual FVector GetMySocketLocation(FName name) const;

	virtual FRotator GetMySocketRotation(FName name) const;
};
