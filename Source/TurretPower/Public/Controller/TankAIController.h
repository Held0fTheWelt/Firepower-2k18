// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "Controller/TurretAIController.h"
#include "TankAIController.generated.h"

class AActor;

/**
 * 
 */
UCLASS()
class TURRETPOWER_API ATankAIController : public ATurretAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Radius in which the tank will behave to stop - about 80 meters (80.00)
	UPROPERTY(EditDefaultsOnly, Category = "Setup") // Consider EditDefaultsOnly
	float DistanceAcceptanceRadius = 80000;
private:
	// Moves the AI towards the player
	void MoveTowardsPlayer(AActor* PlayerTank);
	
	
};
