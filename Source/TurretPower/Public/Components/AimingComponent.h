// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AimingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnFireProjectile, FVector, Location, FRotator, Orientation, float, LaunchSpeed, int8, CannonType, FVector, SpawnScale);

// Enum for TankAimingComponent State
UENUM(Meta = (Bitflags))
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

class UTurret;
class IBarrelInterface;
class UAudioEffectComponent;
class UProjectileObjectPoolComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories = ("Sockets", "Cooking", "Collision", "Asset User Data"))
class TURRETPOWER_API UAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	void Initialize(UTurret* PrimaryTurret, IBarrelInterface* PrimaryBarrel, UTurret* SecondaryTurret, IBarrelInterface* SecondaryBarrel, UAudioEffectComponent* AudioComponent);

	// Aims at the given HitLocation
	void AimAt(FVector HitLocation);

	// Fires a new projectile in the direction, the barrel is currently pointing with Aiming Component
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	// Simple const method that is returníng the current EFiringState
	UFUNCTION(BlueprintCallable, Category = "Firing")
	EFiringState GetFiringState() const;

	UPROPERTY()
	FOnFireProjectile FireProjectile;

	UFUNCTION()
	bool CurrentWeaponIsRocketLauncher() const;

	UPROPERTY(EditAnywhere, Category = "Firing")
	float AttackRadiusDistance = 10000.f;
protected:
	// Firing state of the current Aiming Component
	UPROPERTY(VisibleAnywhere, Category = "State")//BlueprintReadonly
	EFiringState CurrentFiringState = EFiringState::Reloading;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetRoundsLeft() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void SetPrimaryWeapon(bool IsPrimaryWeapon);

	int32 GetRoundsLeftPW() const;
	int32 GetRoundsLeftSW() const;



	/// Sensible starting value of 4000m/s
	// Launchspeed for the tanks projectile
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeedPrimaryWeapon = 4000;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeedSecondaryWeapon = 3000;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	bool SecondaryWeaponIsOptic = true;


	UPROPERTY(VisibleAnywhere, Category = "Firing")
	float CurrentLaunchSpeed = 0;

	// Time to reload for next shot
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSecondsPW = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSecondsSW = 3;


	// Counter to count the last time the primary weapon was fired
	double LastFireTimePW = 0;
	// Counter to count the last time the secondary weapon was fired
	double LastFireTimeSW = 0;

	// Rounds Left to Shoot with Primary Weapon
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	uint32 RoundsLeftPrimaryWeapon = 3;

	// Rounds Left to Shoot with Primary Weapon
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	uint32 RoundsLeftSecondaryWeapon = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	bool IsUsingPrimaryWeaponPart = true;

	FVector SpawnScaleCurrentWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	FVector SpawnScalePrimaryWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	FVector SpawnScaleSecondaryWeapon;

private:
	/// References to the given Entities to use
	UTurret* PrimaryTurret = nullptr;
	IBarrelInterface* PrimaryBarrel = nullptr;
	UTurret* SecondaryTurret = nullptr;
	IBarrelInterface* SecondaryBarrel = nullptr;
	UTurret* CurrentTurret = nullptr;
	IBarrelInterface* CurrentBarrel = nullptr;

	UAudioEffectComponent* AudioComponent = nullptr;
	
	FVector AimDirection;
	double LastFireTime = 0;
	int32 CurrentRoundsLeft = 0;
	float CurrentReloadTime = -1;

	// Constructor
	UAimingComponent();

	// Calculates the vector between forward vector of barrel and aimt
	void CalculateVectorToAim();

	// Calls Barrels Elevate Function with given RelativeSpeed
	void MoveBarrel(float RelativeSpeed);

	// Calls Turrets Rotate Function with given RelativeSpeed
	void RotateTurret(float RelativeSpeed);

	bool IsBarrelMoving() const;

	// Initializes LastFireTime and the current turret and barrel reference
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
