// © 2018 YVES TANAS

#include "TankAIController.h"
#define CURRENTTIMESTAMP GetWorld()->GetTimeSeconds()
void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	#if WITH_EDITOR
		SetFolderPath("Controller/AI/TankController");
	#endif
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MoveTowardsPlayer(PlayerTank);
}

void ATankAIController::MoveTowardsPlayer(AActor* Tank)
{

	MoveToActor(Tank, DistanceAcceptanceRadius);
}
