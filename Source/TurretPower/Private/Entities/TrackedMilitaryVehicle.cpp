// © 2018 YVES TANAS

#include "TrackedMilitaryVehicle.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"

#include "AimingComponent.h"
#include "AudioEffectComponent.h"
#include "FPMovementComponent.h"
#include "SkeletalBarrel.h"
#include "Turret.h"
#include "Track.h"

float ATrackedMilitaryVehicle::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCause)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	auto DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);

	UE_LOG(LogTemp, Warning, TEXT("DamageAmount:%f ,DamageToApply:%i"), DamageAmount, DamageToApply)

		CurrentHealth -= DamageToApply;

	if (CurrentHealth <= 0)
	{
		// Count down lifes
		CurrentLifes--;

		if (CurrentLifes <= 0)
		{
			OnDestruction();
			// TODO explode tank
			OnDeath.Broadcast();
		}
		else
		{
			// Reset Health
			CurrentHealth = StartingHealth;
			// TODO respawn in a small distance of time and place
		}
	}

	return DamageToApply;
}

ATrackedMilitaryVehicle::ATrackedMilitaryVehicle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Initialize();
}

void ATrackedMilitaryVehicle::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingHealth;
	CurrentLifes = StartingLifes;
}

void ATrackedMilitaryVehicle::Initialize()
{
	/// Ensure, all variables have been initialized	
	if (!Body)
		Body = CreateDefaultSubobject<UStaticMeshComponent>(FName("Body"));
	if (!PrimaryTurret)
		PrimaryTurret = CreateDefaultSubobject<UTurret>(FName("PrimaryTurret"));
	if (!PrimaryBarrel)
		PrimaryBarrel = CreateDefaultSubobject<USkeletalBarrel>(FName("PrimaryBarrel"));
	if (!SecondaryTurret)
		SecondaryTurret = CreateDefaultSubobject<UTurret>(FName("SecondaryTurret"));
	if (!SecondaryBarrel)
		SecondaryBarrel = CreateDefaultSubobject<USkeletalBarrel>(FName("SecondaryBarrel"));
	if (!PrimaryDirection)
		PrimaryDirection = CreateDefaultSubobject<UArrowComponent>(FName("Primary Direction"));
	if (!SecondaryDirection)
		SecondaryDirection = CreateDefaultSubobject<UArrowComponent>(FName("Secondary Direction"));
	if (!COM)
		COM = CreateDefaultSubobject<UArrowComponent>(FName("COM"));
	if (!TreadL)
		TreadL = CreateDefaultSubobject<USkeletalMeshComponent>(FName("TreadL"));
	if (!TreadR)
		TreadR = CreateDefaultSubobject<USkeletalMeshComponent>(FName("TreadR"));
	if (!WheelSweep)
		WheelSweep = CreateDefaultSubobject<UStaticMeshComponent>(FName("WheelSweep"));
	if (!TreadsL)
		TreadsL = CreateDefaultSubobject<UTrack>(FName("TreadsL"));
	if (!TreadsR)
		TreadsR = CreateDefaultSubobject<UTrack>(FName("TreadsR"));
	if (!SplineLeft)
		SplineLeft = CreateDefaultSubobject<USplineComponent>(FName("Spline Left"));
	if (!SplineRight)
		SplineRight = CreateDefaultSubobject<USplineComponent>(FName("Spline Right"));

	RootComponent = Body;

	if (WheelSuspensionPoints.Num() != NumberOfWheels)
	{
		WheelSuspensionPoints.SetNum(NumberOfWheels);
	}

	if (WheelSuspensionScale.Num() != NumberOfWheels)
	{
		WheelSuspensionScale.SetNum(NumberOfWheels);
	}


	/// Attach all objects to its given location

	if (Body->GetSocketByName("TurretSocket"))
		PrimaryTurret->AttachToComponent(Body, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("TurretSocket"));
	else
		PrimaryTurret->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);

	if (PrimaryTurret->GetSocketByName("BarrelSocket"))
		PrimaryBarrel->AttachToComponent(PrimaryTurret, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("BarrelSocket"));
	else
		PrimaryBarrel->AttachToComponent(PrimaryTurret, FAttachmentTransformRules::KeepRelativeTransform);

	if (PrimaryTurret->GetSocketByName("SecondarySocket"))
		SecondaryTurret->AttachToComponent(PrimaryTurret, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("SecondarySocket"));
	else
		SecondaryTurret->AttachToComponent(PrimaryTurret, FAttachmentTransformRules::KeepRelativeTransform);

	if (SecondaryTurret->GetSocketByName("SecondaryBarrelSocket"))
		SecondaryBarrel->AttachToComponent(SecondaryTurret, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("SecondaryBarrelSocket"));
	else
		SecondaryBarrel->AttachToComponent(SecondaryTurret, FAttachmentTransformRules::KeepRelativeTransform);

	if (PrimaryBarrel->GetSocketByName("MuzzleSocket"))
		PrimaryDirection->AttachToComponent(PrimaryBarrel, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("MuzzleSocket"));
	else
		PrimaryDirection->AttachToComponent(PrimaryBarrel, FAttachmentTransformRules::KeepRelativeTransform);


	if (SecondaryBarrel->GetSocketByName("MuzzleSocket"))
		SecondaryDirection->AttachToComponent(SecondaryBarrel, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("MuzzleSocket"));
	else
		SecondaryDirection->AttachToComponent(SecondaryBarrel, FAttachmentTransformRules::KeepRelativeTransform);

	SecondaryDirection->SetWorldScale3D(FVector(.5f));

	COM->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	
	WheelSweep->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	
	TreadL->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	TreadR->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	

	if (Body->GetSocketByName("LeftTrackSocket"))
	{
		TreadsL->AttachToComponent(Body, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("LeftTrackSocket"));
		SplineLeft->AttachToComponent(Body, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("LeftTrackSocket"));
	}
	else
	{
		TreadsL->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
		SplineLeft->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	}

	TreadsL->Initialize(SplineLeft, SplinePoints, SplineTangents, TreadsOnSide, &WheelSuspensionPoints, &WheelSuspensionScale, TreadL);

	if (Body->GetSocketByName("RightTrackSocket"))
	{
		TreadsR->AttachToComponent(Body, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RightTrackSocket"));
		SplineRight->AttachToComponent(Body, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RightTrackSocket"));
	}
	else
	{
		TreadsR->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
		SplineRight->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	}

	TreadsR->Initialize(SplineRight, SplinePoints, SplineTangents, TreadsOnSide, &WheelSuspensionPoints, &WheelSuspensionScale, TreadR);

	/// Toggle the Arrows Debug visibility with ShowArrows value
	if (!ShowArrows)
	{
		COM->SetVisibility(false);
		PrimaryDirection->SetVisibility(false);
		SecondaryDirection->SetVisibility(false);
	}
	else
	{
		COM->SetVisibility(true);
		PrimaryDirection->SetVisibility(true);
		SecondaryDirection->SetVisibility(true);
	}
	/// ensure, we have an AudioComponent setup
	if (!AudioComponent)
		AudioComponent = CreateDefaultSubobject<UAudioEffectComponent>(FName("Audio Component"));
		
	/// ensure, we have an AimingComponent setup
	if (!AimingComponent)
		AimingComponent = CreateDefaultSubobject<UAimingComponent>(FName("Aiming Component"));

	/// Initialize the Aiming Component
	AimingComponent->Initialize(PrimaryTurret, PrimaryBarrel, SecondaryTurret, SecondaryBarrel, AudioComponent);

	if (!MovementComponent)
		MovementComponent = CreateDefaultSubobject<UFPMovementComponent>(FName("MovementComponent"));

	MovementComponent->Initialize(TreadsL, TreadsR);

#if WITH_EDITOR
	//	SetFolderPath("Entities");//TODO +WorldOutlinerFolderName.ToString());
#endif
}

USkeletalBarrel* ATrackedMilitaryVehicle::GetPrimaryBarrel()
{
	return PrimaryBarrel;
}

USkeletalBarrel * ATrackedMilitaryVehicle::GetSecondaryBarrel()
{
	return SecondaryBarrel;
}

UAimingComponent * ATrackedMilitaryVehicle::GetAimingComponent()
{
	return AimingComponent;
}

UTrack * ATrackedMilitaryVehicle::GetTreadsLeft()
{
	return TreadsL;
}

UTrack * ATrackedMilitaryVehicle::GetTreadsRight()
{
	return TreadsR;
}

UTurret * ATrackedMilitaryVehicle::GetPrimaryTurret()
{
	return PrimaryTurret;
}

float ATrackedMilitaryVehicle::GetHealthPercent() const
{
	return float(CurrentHealth) / float(StartingHealth);
}
int ATrackedMilitaryVehicle::GetLifesLeft() const
{
	return CurrentLifes;
}

UFPMovementComponent * ATrackedMilitaryVehicle::GetFPMovementComponent()
{
	return MovementComponent;
}

void ATrackedMilitaryVehicle::SetVisibility(bool Visibility)
{
	VisibleToPlayer(Visibility);
}
