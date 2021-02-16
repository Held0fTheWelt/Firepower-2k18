// © 2018 YVES TANAS

#include "TurretAIController.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TrackedMilitaryVehicle.h"
#include "TrackedMilitaryEntity.h"
#include "AimingComponent.h"

#define CURRENTTIMESTAMP GetWorld()->GetTimeSeconds()
#define OWNER *(GetOwner()->GetName())

// Called on Start, checking for NullReferences
void ATurretAIController::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("%f: AIController Setup on Entity"), CURRENTTIMESTAMP)

	PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	#if WITH_EDITOR
		SetFolderPath("Controller/AI/TurretController");
	#endif
}

// High Level Tick method to chase the player
void ATurretAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto ControlledTank = GetPawn();

	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: AIController could not find Player Tank"), CURRENTTIMESTAMP)
			return;
	}
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: AIController could not find controlled turret"), CURRENTTIMESTAMP)
			return;
	}

	auto AimingComponent = ControlledTank->FindComponentByClass<UAimingComponent>();
	if (!AimingComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: AIController could not find AimingComponent"), CURRENTTIMESTAMP)
			return;
	}

	if (ControlledTank->GetHorizontalDistanceTo(PlayerTank) >= AimingComponent->AttackRadiusDistance) return;

	AimTowardsPlayer(AimingComponent, PlayerTank);

	// if Locked
	if (AimingComponent->GetFiringState() == EFiringState::Locked
		|| AimingComponent->GetFiringState() == EFiringState::Aiming && AimingComponent->CurrentWeaponIsRocketLauncher())
		// Check if is in Range:
			FireTowardsPlayer(AimingComponent);

	

	//UE_LOG(LogTemp, Warning, TEXT("%f: AIController is %f away"), CURRENTTIMESTAMP, ControlledTank->GetHorizontalDistanceTo(PlayerTank))
}

void ATurretAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);

	if (!(InPawn)) return;

	if (InPawn->IsA(ATrackedMilitaryVehicle::StaticClass()))
	{
		auto PossessedTank = Cast<ATrackedMilitaryVehicle>(InPawn);
		if (!PossessedTank) return;
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATurretAIController::OnPossessedTankDeath);
	}

	if (InPawn->IsA(ATrackedMilitaryEntity::StaticClass()))
	{
		auto PossessedTank = Cast<ATrackedMilitaryEntity>(InPawn);
		if (!PossessedTank) return;
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATurretAIController::OnPossessedTankDeath);
	}
}


void ATurretAIController::OnPossessedTankDeath()
{
	if (!GetPawn()) return;

	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATurretAIController::FireTowardsPlayer(UAimingComponent* AimingComponent)
{
	AimingComponent->Fire();
}
// Calls the controlled tank to aim at the players tank actor location
void ATurretAIController::AimTowardsPlayer(UAimingComponent* AimingComponent, AActor* CurrentPlayerTank) {

	AimingComponent->AimAt(CurrentPlayerTank->GetActorLocation());
}