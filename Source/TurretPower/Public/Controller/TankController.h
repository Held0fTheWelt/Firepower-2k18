// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankController.generated.h"

class ATrackedMilitaryVehicle;
class UAimingComponent;

UCLASS()
class TURRETPOWER_API ATankController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		void OnPossessedTankDeath();

protected:
	/// We don't need an implementation wit BlueprintImplementableEvent
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UAimingComponent* AimingComponentRef);

	/// We don't need an implementation wit BlueprintImplementableEvent
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundTankActor(ATrackedMilitaryVehicle* TankRef);

private:
	// Debug Message on Start
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetPawn(APawn* InPawn) override;

	// Starts the tank moving the barrel so that the shot would hit where the crosshair intersects the world
	void AimTowardsCrosshair();

	// Get world location of linetrace through crosshair, true if hits landscape
	bool GetSightRayHitLocation(FVector& OUT_HitLocation) const;
	// defines a Hitresult, makes a linetrace, and returns, if something is hit
	// HitLocation is reference type and used as OUT Parameter with changed values
	bool GetLookVectorHitDirection(FVector LookDirection, FVector& HitLocation) const;
	// Returns the current LookDirection if true
	bool GetLookDirection(FVector2D ScreenPosition, FVector& LookDirection) const;

	UAimingComponent* AimingComponent;

	

	// The x location of the crosshair as % value between 0 (=0%) and 1 (=100%)
	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5;
	// The y location of the crosshair as % value between 0 (=0%) and 1 (=100%)
	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.3333;
	// The range of the line trace
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0;
};
