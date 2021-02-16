// © 2018 YVES TANAS

#include "AimingComponent.h"

#include "Core/Public/Math/Vector.h"
#include "Core/Public/Math/Rotator.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

#include "Turret.h"
#include "Barrel.h"
#include "AudioEffectComponent.h"
#include "TrackedMilitaryEntity.h"
#include "TrackedMilitaryVehicle.h"

#define CURRENTTIMESTAMP GetWorld()->GetTimeSeconds()
#define OWNER *(GetOwner()->GetName())


// calculates an aiming solution for the given HitLocation and, if found, rotates the barrel 
// and turret into the needed position, if possible
void UAimingComponent::AimAt(FVector HitLocation)
{
	// if there is no barrel or turret, return for safety reason
	if (!CurrentBarrel)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: Barrel is missing on: %s. PrimaryWeapon: %i"), CURRENTTIMESTAMP, OWNER, IsUsingPrimaryWeaponPart)
			return;
	}
	if (!CurrentTurret)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: Barrel is missing on: %s. PrimaryWeapon: %i"), CURRENTTIMESTAMP, OWNER, IsUsingPrimaryWeaponPart)
			return;
	}
	// Initialize an OutVelocity vector for launching the projectile
	FVector OutLaunchVelocity;
	// Get the barrels socket location for projectiles
	FVector StartLocation = CurrentBarrel->GetMySocketLocation(FName("Projectile")); //TODO: Get the given Socket Location !!!

	bool isRocket = false;
	if (CurrentBarrel->GetBarrelType() == EBarrelType::Rocket)
		isRocket = true;
	
	// calculate the OutLaunchVelocity with the known information of our
	// StartLocation, the aimed HitLocation and our Launchspeed
	// We will think about High-Arc later - might be interesting to play with special vehicles using this
	// Theese could fire from out of sight !
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		CurrentLaunchSpeed,
		isRocket, // High-Arc
		0, // Collision Radius
		0, // Override Gravity
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	//UE_LOG(LogTemp, Warning, TEXT("%f: Has AimSolution: %i"), CURRENTTIMESTAMP, bHaveAimSolution)

	// if we have an aim solution
	if (bHaveAimSolution)
	{
		// Get the Normal Vector for this direction
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		// Calculate the vector to aim
		CalculateVectorToAim();

	}
}


void UAimingComponent::Fire()
{
	// if there is no barrel or turret, return for safety reason
	if (!CurrentBarrel)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: Barrel is missing on: %s. PrimaryWeapon: %i"), CURRENTTIMESTAMP, OWNER, IsUsingPrimaryWeaponPart)
			return;
	}
//	UE_LOG(LogTemp, Warning, TEXT("%f: Current Firing State: %i"), CURRENTTIMESTAMP, CurrentFiringState.GetValue())
	if (CurrentFiringState == EFiringState::Aiming || CurrentFiringState == EFiringState::Locked) {

		

		
		// Fire projectile from object pool
		FireProjectile.Broadcast(CurrentBarrel->GetMySocketLocation(FName("MuzzleSocket")), CurrentBarrel->GetMySocketRotation(FName("MuzzleSocket")), CurrentLaunchSpeed, int8(CurrentBarrel->GetBarrelType()), SpawnScaleCurrentWeapon);

		if (IsUsingPrimaryWeaponPart)
		{
			LastFireTime = LastFireTimePW = CURRENTTIMESTAMP;
			RoundsLeftPrimaryWeapon--;
		}
		else
		{
			LastFireTime = LastFireTimeSW = CURRENTTIMESTAMP;
			RoundsLeftSecondaryWeapon--;
		}

		AudioComponent->PlayAudioEffect();
		
		if (GetOwner()->IsA(ATrackedMilitaryVehicle::StaticClass()))
		{
			Cast<ATrackedMilitaryVehicle>(GetOwner())->OnBarrelFire();
		}

		if (GetOwner()->IsA(ATrackedMilitaryEntity::StaticClass()))
		{
			Cast<ATrackedMilitaryEntity>(GetOwner())->OnBarrelFire();			
		}
	}
}


void UAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Set Current Firing State to given state
	if (CurrentRoundsLeft <= 0)
		CurrentFiringState = EFiringState::OutOfAmmo;
	else if ((CURRENTTIMESTAMP - LastFireTime) < CurrentReloadTime)
		CurrentFiringState = EFiringState::Reloading;
	else if (!IsBarrelMoving())
		CurrentFiringState = EFiringState::Aiming;
	else
		CurrentFiringState = EFiringState::Locked;
}
// Initializes LastFireTime and the current turret and barrel reference
void UAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// Reload at start needed
	LastFireTime = CURRENTTIMESTAMP;
	
	SetPrimaryWeapon(true);
}


void UAimingComponent::SetPrimaryWeapon(bool IsPrimaryWeapon)
{
	if (IsPrimaryWeapon)
	{

		UE_LOG(LogTemp, Warning, TEXT("%f: Rounds on Primary: %i"), CURRENTTIMESTAMP, RoundsLeftPrimaryWeapon)
		CurrentTurret = PrimaryTurret;
		CurrentBarrel = PrimaryBarrel;
		CurrentRoundsLeft = RoundsLeftPrimaryWeapon;
		CurrentReloadTime = ReloadTimeInSecondsPW;
		CurrentLaunchSpeed = LaunchSpeedPrimaryWeapon;
		SpawnScaleCurrentWeapon = SpawnScalePrimaryWeapon;
	}
	else
	{
		CurrentTurret = SecondaryTurret;
		CurrentBarrel = SecondaryBarrel;
		CurrentRoundsLeft = RoundsLeftSecondaryWeapon;
		CurrentReloadTime = ReloadTimeInSecondsSW;
		CurrentLaunchSpeed = LaunchSpeedSecondaryWeapon;
		SpawnScaleCurrentWeapon = SpawnScaleSecondaryWeapon;
	}
	IsUsingPrimaryWeaponPart = IsPrimaryWeapon;
}
// Calculates Vector between barrels forward and aim
// Calls to Rotate Turret and elevate Barrel
void UAimingComponent::CalculateVectorToAim()
{
	/// work out difference between current barrel rotation and aim direction

	// Get the current barrels FRotator
	auto BarrelRotator = CurrentBarrel->GetMyForwardVector().Rotation();
	// Get the rotation of our aim
	auto AimToDirection = AimDirection.Rotation();
	// calculate the rotator between theese two rotators
	auto DeltaRotator = AimToDirection - BarrelRotator;

	// check if rotation should be be changed to opposite direction
	if (FMath::Abs(DeltaRotator.Yaw) > 180)
		DeltaRotator.Yaw = -DeltaRotator.Yaw;
	// rotate the turret the right amount this frame by using DeltaRotators yaw value
	RotateTurret(DeltaRotator.Yaw);

	// elevate barrel the right amount this frame by using DeltaRotators pitch value
	MoveBarrel(DeltaRotator.Pitch);
}
// Initializes the AimingComponent by setting up all reference values
/// If there is a null reference, which shouldn't happen, we might want to solve this more differently
/// Since here we just return, because this really should never happen, and the ensure will point us here, if it does.
/// It's split up, to easily see the problematic reference
void UAimingComponent::Initialize(UTurret * pTurret, IBarrelInterface * pBarrel, UTurret * sTurret, IBarrelInterface * sBarrel, UAudioEffectComponent* audioComponent)
{
	if (!ensure(pTurret)) return;
	if (!ensure(pBarrel)) return;
	if (!ensure(sTurret)) return;
	if (!ensure(sBarrel)) return;

	PrimaryTurret = pTurret;
	PrimaryBarrel = pBarrel;
	SecondaryTurret = sTurret;
	SecondaryBarrel = sBarrel;
	AudioComponent = audioComponent;
}
void UAimingComponent::MoveBarrel(float RelativeSpeed) { CurrentBarrel->Elevate(RelativeSpeed); }
bool UAimingComponent::IsBarrelMoving() const
{
	// Prevent Shooting to show being locked and send a message to UELOG
	if (!CurrentBarrel)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: Barrel is missing on: %s. PrimaryWeapon: %i"), CURRENTTIMESTAMP, OWNER, IsUsingPrimaryWeaponPart)
			return true;
	}
	auto BarrelForward = CurrentBarrel->GetMyForwardVector();
	return BarrelForward.Equals(AimDirection, 0.1);
}
void UAimingComponent::RotateTurret(float RelativeSpeed) { CurrentTurret->Rotate(RelativeSpeed); }
EFiringState UAimingComponent::GetFiringState() const { return CurrentFiringState; }
int32 UAimingComponent::GetRoundsLeft() const {
	if (IsUsingPrimaryWeaponPart)
		return RoundsLeftPrimaryWeapon;
	else
		return RoundsLeftSecondaryWeapon;
}
int32 UAimingComponent::GetRoundsLeftPW() const
{
	return RoundsLeftPrimaryWeapon;
}
int32 UAimingComponent::GetRoundsLeftSW() const
{
	return RoundsLeftSecondaryWeapon;
}
bool UAimingComponent::CurrentWeaponIsRocketLauncher() const
{
	if (CurrentBarrel->GetBarrelType() == EBarrelType::Rocket)
		return true;
	return false;
}
UAimingComponent::UAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}