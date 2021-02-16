// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BarrelInterface.h"
#include "Barrel.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnGatlingFire);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TURRETPOWER_API UBarrel : public UStaticMeshComponent, public IBarrelInterface
{
	GENERATED_BODY()
	
public:
	// Elevates the Barrel by the given RelativeSpeed. Negative values will lower, positive values will lift the barrel
	virtual void Elevate(float RelativeSpeed) override;

private:
	// Value to change the maximumg degrees per second, the barrel can elevate
	UPROPERTY(EditDefaultsOnly)
	float MaxDegreesPerSecond = 10;

	// Value to change the maximum elevation degrees. Upon this tank cannot elevate its barrel
	UPROPERTY(EditDefaultsOnly)
	float MaxElevationDegrees = 40;

	// Value to change the minimum elevation degrees. Below this tank cannot elevate its barrel
	UPROPERTY(EditDefaultsOnly)
	float MinElevationDegrees = 0;

	UPROPERTY(EditDefaultsOnly)
	float BarrelRotationSpeed = 5;

	float CurrentRotation = 0.f;
public:
	// Firing state of the current Aiming Component
	UPROPERTY(EditAnywhere, Category = "Setup")
	EBarrelType BarrelType = EBarrelType::Cannon;

	UFUNCTION()
	virtual EBarrelType GetBarrelType() const override;

	virtual FVector GetMyForwardVector() const override;
	
	virtual FVector GetMySocketLocation(FName name) const override;
	
	virtual FRotator GetMySocketRotation(FName name) const override;
};
