// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Turret.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TURRETPOWER_API UTurret : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Rotates the turret by the given RelativeSpeed. Negative values will turn left, positive values will turn right
	void Rotate(float RelativeSpeed);

private:
	// Value to change the maximumg degrees per second, the turret can turn
	UPROPERTY(EditDefaultsOnly)
	float MaxDegreesPerSecond = 25;
};
