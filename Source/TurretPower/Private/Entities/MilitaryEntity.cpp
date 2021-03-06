// © 2018 YVES TANAS

#include "MilitaryEntity.h"


#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

#include "AimingComponent.h"
#include "AudioEffectComponent.h"
#include "Turret.h"
#include "BarrelInterface.h"
#include "AbstractBarrel.h"
float AMilitaryEntity::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCause)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	auto DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);
	//UE_LOG(LogTemp, Warning, TEXT("DamageAmount:%f ,DamageToApply:%i"), DamageAmount, DamageToApply)
	CurrentHealth -= DamageToApply;

	if (CurrentHealth <= 0)
	{
		// Count down lifes
		CurrentLifes--;

		if (CurrentLifes <= 0)
		{
			OnDestruction();			
			OnDeath.Broadcast();
		}
		else
		{
			CurrentHealth = StartingHealth;
			// TODO respawn in a small distance of time and place
		}
	}

	return DamageToApply;
}



void AMilitaryEntity::Initialize()
{
	if (!Body)
		Body = CreateDefaultSubobject<UStaticMeshComponent>(FName("Body"));
	if (!PrimaryTurret)
		PrimaryTurret = CreateDefaultSubobject<UTurret>(FName("PrimaryTurret"));
	if (!SecondaryTurret)
		SecondaryTurret = CreateDefaultSubobject<UTurret>(FName("SecondaryTurret"));
	if (!PrimaryDirection)
		PrimaryDirection = CreateDefaultSubobject<UArrowComponent>(FName("Primary Direction"));
	if (!SecondaryDirection)
		SecondaryDirection = CreateDefaultSubobject<UArrowComponent>(FName("Secondary Direction"));

	RootComponent = Body;

	/// Attach all objects to its given location

	if (Body->GetSocketByName("TurretSocket"))
		PrimaryTurret->AttachToComponent(Body, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("TurretSocket"));
	else
		PrimaryTurret->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);

	if (PrimaryTurret->GetSocketByName("SecondarySocket"))
		SecondaryTurret->AttachToComponent(PrimaryTurret, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("SecondarySocket"));
	else
		SecondaryTurret->AttachToComponent(PrimaryTurret, FAttachmentTransformRules::KeepRelativeTransform);

	if (PrimaryTurret)
	{
		if (PrimaryTurret->GetSocketByName("BarrelSocket"))
			PrimaryDirection->AttachToComponent(PrimaryTurret, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("BarrelSocket"));
		else
			PrimaryDirection->AttachToComponent(PrimaryTurret, FAttachmentTransformRules::KeepRelativeTransform);
		/// Toggle the Arrows Debug visibility with ShowArrows value
		if (ShowArrows)
			PrimaryDirection->SetVisibility(true);
		else
			PrimaryDirection->SetVisibility(false);
	}

	if (SecondaryTurret)
	{
		if (SecondaryTurret && SecondaryTurret->GetSocketByName("BarrelSocket"))
			SecondaryDirection->AttachToComponent(SecondaryTurret, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("BarrelSocket"));
		else
			SecondaryDirection->AttachToComponent(SecondaryTurret, FAttachmentTransformRules::KeepRelativeTransform);
		SecondaryDirection->SetWorldScale3D(FVector(.5f));
		/// Toggle the Arrows Debug visibility with ShowArrows value
		if (ShowArrows)
			SecondaryDirection->SetVisibility(true);
		else
			SecondaryDirection->SetVisibility(false);
	}		

	/// ensure, we have an AudioComponent setup
	if (!AudioComponent)
		AudioComponent = CreateDefaultSubobject<UAudioEffectComponent>(FName("Audio Component"));

	/// ensure, we have an AimingComponent setup
	if (!AimingComponent)
		AimingComponent = CreateDefaultSubobject<UAimingComponent>(FName("Aiming Component"));

	/// Initialize the Aiming Component
//	AimingComponent->Initialize(PrimaryTurret, PrimaryBarrel, SecondaryTurret, SecondaryBarrel, AudioComponent);

#if WITH_EDITOR
	//	SetFolderPath("Entities");//TODO +WorldOutlinerFolderName.ToString());
#endif
}

// BeginPlay innitializes Health, Life, Ammo and Fuel
// TODO Ammo and Fuel implementation
void AMilitaryEntity::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingHealth;
	CurrentLifes = StartingLifes;
}

/// Getter Methods

UTurret * AMilitaryEntity::GetPrimaryTurret()	{	return PrimaryTurret;	}
UAimingComponent * AMilitaryEntity::GetAimingComponent(){	return AimingComponent;	}
float AMilitaryEntity::GetHealthPercent() const { return float(CurrentHealth) / float(StartingHealth); }
int AMilitaryEntity::GetLifesLeft() const	{	return CurrentLifes;	}
AActor* AMilitaryEntity::GetLockedOnTarget() const {	return LockedOnTarget;	}

/// Setter Methods

void AMilitaryEntity::SetLockedOnTarget(AActor* Target)	{	LockedOnTarget = Target; }
void AMilitaryEntity::SetVisibility(bool Visibility) {	VisibleToPlayer(Visibility); }

/// Constructor

AMilitaryEntity::AMilitaryEntity()
{
	PrimaryActorTick.bCanEverTick = true;
	Initialize();
}
