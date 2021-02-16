// // © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurretAIController.generated.h"

class UAimingComponent;

/** AIController for the given turret is handling the same way, the player does */
UCLASS()
class TURRETPOWER_API ATurretAIController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void OnPossessedTankDeath();

protected:
	// Debug Message on Start
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor* PlayerTank;
private:


	virtual void SetPawn(APawn* InPawn) override;

	// Starts the tank moving the barrel so that the shot would hit where the crosshair intersects the world
	void AimTowardsPlayer(UAimingComponent* AimingComponent, AActor* PlayerTank);
	// Fires a projectile towards the player location
	void FireTowardsPlayer(UAimingComponent* AimingComponent);

	
};
