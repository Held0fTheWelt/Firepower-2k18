// © 2018 YVES TANAS

#include "TankController.h"

/// GetViewportSize
#include "Engine.h"
#include "TrackedMilitaryVehicle.h"
#include "AimingComponent.h"

#define CURRENTTIMESTAMP GetWorld()->GetTimeSeconds()
#define CURRENTPAWNNAME *(GetPawn()->GetName())

// Check for instance variables, warn if something went wrong
void ATankController::BeginPlay() {

	Super::BeginPlay();

	if (!ensure(GetPawn())) return;
	AimingComponent = GetPawn()->FindComponentByClass<UAimingComponent>();
	if (!ensure(AimingComponent))
	{
		// inform about missing tank on player control - update blueprint, if this is shown
		UE_LOG(LogTemp, Warning, TEXT("%f: AimingComponent not found on tank %s!"), CURRENTTIMESTAMP, CURRENTPAWNNAME)
	}
	else
	{
		FoundAimingComponent(AimingComponent);
		// inform about everything set okay on the player controller
		UE_LOG(LogTemp, Warning, TEXT("%f: AimingComponent found on %s"), CURRENTTIMESTAMP, CURRENTPAWNNAME)
	}
}

void ATankController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);

	if (!(InPawn)) return;

	auto PossessedTank = Cast<ATrackedMilitaryVehicle>(InPawn);

	if (!ensure(PossessedTank)) return;

	// Subscribe to the tanks death event
	PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankController::OnPossessedTankDeath);
}

void ATankController::OnPossessedTankDeath()
{
	StartSpectatingOnly();
}

// The tick method is high level architecture
// In here, we will only see game flow, but no game flow implementation
void ATankController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Aim towards crosshair
	AimTowardsCrosshair();
}

// method aims towards crosshair, if needed
void ATankController::AimTowardsCrosshair() {
	// this is a runtime check and might be null sometimes, so just leave and don't break the code
	if (!GetPawn()) return;
	// If we don't have a controlled tank, we want to get out of here
	if (!(AimingComponent)) { return; }

	/// Get World Location if linetrace through crosshair
	// Out Parameter
	FVector HitLocation;
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
	// Has Sideeffect - Raytraces and returns a HitLocation
	if (bGotHitLocation)
	{
		/// Tell controlled tank to aim at this point
		AimingComponent->AimAt(HitLocation);
	}
}

// Get world location of linetrace through crosshair, true if hits landscape
bool ATankController::GetSightRayHitLocation(FVector & HitLocation) const
{
	/// Find the crosshair position
	// Size of the current Viewport
	int32 ViewPortSizeX, ViewPortSizeY;
	// Get the viewport size
	GetViewportSize(ViewPortSizeX, ViewPortSizeY);
	// translate screen location to the current known crosshair location
	auto ScreenLocation = FVector2D(ViewPortSizeX * CrosshairXLocation, ViewPortSizeY * CrosshairYLocation);

	/// De-Project the screen position of the crosshair to a world position
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		/// Line trace along that LookDirection , and see what it hits (up to maximum range)
		return GetLookVectorHitDirection(LookDirection, HitLocation);
	}
	return false;
}

// defines a Hitresult, makes a linetrace, and returns, if something is hit
// HitLocation is reference type and used as OUT Parameter with changed values
bool ATankController::GetLookVectorHitDirection(FVector LookDirection, FVector& HitLocation) const
{
	// define a new HitResult
	FHitResult HitResult;
	// Get the Start Location from the current PlayerCameraManagers CameraLocation
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	// Calculate the Endlocation, defined by StartLocation and the LookDirection multiplied by the LineTraceRange
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	// Make a LineTraceSingleChannel for the given Values and look for anything, that is visible
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Camera
	))
	{
		// if it hit something, enter the current HitResult location
		HitLocation = HitResult.Location;
		// return true
		return true;
	}
	// otherwise set Vector 0 to the Hitlocation to intend, nothing was hit
	HitLocation = FVector(0);
	// return false
	return false;
}

// Returns the LookDirection, if everything is okay.
// LookDirection is reference type and used as OUT Parameter with changed values
bool ATankController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // To be discarded, but must be put in

	// Deproject the screen projection of the camera to the world projection in the current scene
	DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection
	);

	// return true, because everything is fine
	return true;
}