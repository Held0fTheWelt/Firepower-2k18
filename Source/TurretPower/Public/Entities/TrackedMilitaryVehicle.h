// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrackedMilitaryVehicle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDeathDelegate);

class UArrowComponent;
class UAimingComponent;
class UAudioEffectComponent;
class UFPMovementComponent;
class UProjectileObjectPoolComponent;
class UTurret;
class USkeletalBarrel;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class USplineComponent;
class UTrack;

UCLASS()
class TURRETPOWER_API ATrackedMilitaryVehicle : public APawn
{
	GENERATED_BODY()
public:	
	// Sets default values for this pawn's properties
	ATrackedMilitaryVehicle();

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	bool isUsingFirstPerson = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnBarrelFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnDestruction();

	// Call this in Blueprint inside the Construction Script to make Composition possible
	UFUNCTION(BlueprintCallable, Category = "Setup")
	virtual void Initialize();

	// Call this in Blueprint inside the Construction Script to make Composition possible
	UFUNCTION(BlueprintCallable, Category = "Setup")
	USkeletalBarrel* GetPrimaryBarrel();

	// Call this in Blueprint inside the Construction Script to make Composition possible
	UFUNCTION(BlueprintCallable, Category = "Setup")
	USkeletalBarrel* GetSecondaryBarrel();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	UAimingComponent* GetAimingComponent();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	UTrack* GetTreadsLeft();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	UTrack* GetTreadsRight();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	UTurret* GetPrimaryTurret();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	UFPMovementComponent* GetFPMovementComponent();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetVisibility(bool Visibility);

	// is Pawn is Visible to player
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void VisibleToPlayer(bool isVisible);

protected:
	// Shows Debug Direction Arrows
	UPROPERTY(EditAnywhere, Category = "Debug")
		bool ShowArrows = true;

	//	UPROPERTY(EditAnywhere, Category = "Debug")
	//	FName WorldOutlinerFolderName = "Default";

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Body = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTurret* PrimaryTurret = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalBarrel* PrimaryBarrel = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTurret* SecondaryTurret = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalBarrel* SecondaryBarrel = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArrowComponent* PrimaryDirection = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArrowComponent* SecondaryDirection = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArrowComponent* COM = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAimingComponent* AimingComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioEffectComponent* AudioComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* TreadL = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* TreadR = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* WheelSweep = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTrack* TreadsL = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTrack* TreadsR = nullptr;
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Components")
	USplineComponent* SplineLeft = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	USplineComponent* SplineRight = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	TArray<FVector> SplinePoints;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	TArray<FVector> SplineTangents;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TreadsOnSide = 64.f;

	// First Wheel will be Sprocket, Last Wheel will be Idler
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int NumberOfWheels = 8;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TArray<FVector> WheelSuspensionPoints;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TArray<FVector> WheelSuspensionScale;
	

	virtual void BeginPlay() override;
public:

	FTankDeathDelegate OnDeath;

	//Called by the engine, when actor damage is dealt
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCause) override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Lifes")
	int GetLifesLeft() const;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingLifes = 0;

	UPROPERTY(VisibleAnywhere, Category = "Lifes")
	int32 CurrentLifes = 0;

	UFPMovementComponent* MovementComponent = nullptr;

	
};
