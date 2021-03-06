// © 2018 YVES TANAS

#include "TrackedMilitaryEntity.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

#include "AimingComponent.h"
#include "AudioEffectComponent.h"
#include "Turret.h"
#include "Barrel.h"


float ATrackedMilitaryEntity::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCause)
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

ATrackedMilitaryEntity::ATrackedMilitaryEntity()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Initialize();
}
float ATrackedMilitaryEntity::GetHealthPercent() const
{
	return float(CurrentHealth) / float(StartingHealth);
}
int ATrackedMilitaryEntity::GetLifesLeft() const
{
	return CurrentLifes;
}

AActor* ATrackedMilitaryEntity::GetLockedOnTarget()
{
	return LockedOnTarget;
}

void ATrackedMilitaryEntity::SetLockedOnTarget(AActor* Target)
{
	LockedOnTarget = Target;
}

void ATrackedMilitaryEntity::Initialize()
{
	/// Ensure, all variables have been initialized
	// ensure, we have a body
	if (!(Body))
		Body = CreateDefaultSubobject<UStaticMeshComponent>(FName("Body"));
	// ensure, the primary turret is added
	if (!(PrimaryTurret))
		PrimaryTurret = CreateDefaultSubobject<UTurret>(FName("PrimaryTurret"));
	// ensure, the primary barrel is added
	if (!(PrimaryBarrel))
		PrimaryBarrel = CreateDefaultSubobject<UBarrel>(FName("PrimaryBarrel"));
	// ensure, the secondary turret is added
	if (!(SecondaryTurret))
		SecondaryTurret = CreateDefaultSubobject<UTurret>(FName("SecondaryTurret"));
	// ensure the secondary barrel is added
	if (!(SecondaryBarrel))
		SecondaryBarrel = CreateDefaultSubobject<UBarrel>(FName("SecondaryBarrel"));
	// ensure, the primary arrow is added
	if (!(PrimaryDirection))
		PrimaryDirection = CreateDefaultSubobject<UArrowComponent>(FName("Primary Direction"));
	// ensure the secondary arrow is added
	if (!(SecondaryDirection))
		SecondaryDirection = CreateDefaultSubobject<UArrowComponent>(FName("Secondary Direction"));

	RootComponent = Body;

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

	/// Toggle the Arrows Debug visibility with ShowArrows value
	if (!ShowArrows)
	{
		PrimaryDirection->SetVisibility(false);
		SecondaryDirection->SetVisibility(false);
	}
	else
	{
		PrimaryDirection->SetVisibility(true);
		SecondaryDirection->SetVisibility(true);
	}
	/// ensure, we have an AudioComponent setup
	if (!(AudioComponent))
		AudioComponent = CreateDefaultSubobject<UAudioEffectComponent>(FName("Audio Component"));



	/// ensure, we have an AimingComponent setup
	if (!(AimingComponent))
		AimingComponent = CreateDefaultSubobject<UAimingComponent>(FName("Aiming Component"));

	/// Initialize the Aiming Component
	AimingComponent->Initialize(PrimaryTurret, PrimaryBarrel, SecondaryTurret, SecondaryBarrel, AudioComponent);

#if WITH_EDITOR
	//	SetFolderPath("Entities");//TODO +WorldOutlinerFolderName.ToString());
#endif
}
void ATrackedMilitaryEntity::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingHealth;
	CurrentLifes = StartingLifes;
}
UBarrel* ATrackedMilitaryEntity::GetPrimaryBarrel()
{
	return PrimaryBarrel;
}

UTurret * ATrackedMilitaryEntity::GetPrimaryTurret()
{
	return PrimaryTurret;
}

UAimingComponent * ATrackedMilitaryEntity::GetAimingComponent()
{
	return AimingComponent;
}

void ATrackedMilitaryEntity::SetVisibility(bool Visibility)
{
	VisibleToPlayer(Visibility);
}
