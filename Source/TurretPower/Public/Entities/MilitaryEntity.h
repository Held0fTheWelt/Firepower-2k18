// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MilitaryEntity.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMilitaryEntityDelegateDeath);

class UArrowComponent;
class UAimingComponent;
class UAudioEffectComponent;
class UTurret;
class UAbstractBarrel;
class IBarrelInterface;

UCLASS(abstract)
class TURRETPOWER_API AMilitaryEntity : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMilitaryEntity();

	FMilitaryEntityDelegateDeath OnDeath;

	// Call this in Blueprint inside the Construction Script to make Composition possible
	UFUNCTION(BlueprintCallable, Category = "Setup")
	virtual void Initialize();

#pragma region BlueprintImplementableEvents
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnBarrelFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnDestruction();

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnTurretRotate(bool IsPrimaryTurret);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnBarrelMove(bool IsPrimaryTurret);
#pragma endregion to handle special behaviour on  action
	

	// Return value, if actor is locked on
	UFUNCTION(BlueprintCallable, Category = "Setup")
	AActor* GetLockedOnTarget() const;

	void SetLockedOnTarget(AActor* Target);

	UAbstractBarrel* PrimaryBarrel = nullptr;

	// Blueprint Callable implementation for inherited primary turret
	UFUNCTION(BlueprintCallable, Category = "Setup")
	UTurret* GetPrimaryTurret();

	// Blueprint Callable implementation for inherited Aiming Component
	UFUNCTION(BlueprintCallable, Category = "Setup")
	UAimingComponent* GetAimingComponent();

	/// Visibility functionality

	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetVisibility(bool Visibility);

	// is Pawn is Visible to player
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void VisibleToPlayer(bool isVisible);

protected:

	AActor* LockedOnTarget;

private:
	virtual void BeginPlay() override;

#pragma region Debug
	// Shows Debug Direction Arrows
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool ShowArrows = true;
#pragma endregion information seen on the entity

#pragma region Components and Meshes

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Body = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTurret* PrimaryTurret = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTurret* SecondaryTurret = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArrowComponent* PrimaryDirection = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArrowComponent* SecondaryDirection = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAimingComponent* AimingComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioEffectComponent* AudioComponent = nullptr;
#pragma endregion of this actors military entity without barrels or extras
	
#pragma region Life
protected:
	// Called by the engine, when actor damage is dealt
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCause) override;
	
	// Health Percentage for the current actor
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	// current lifes left
	UFUNCTION(BlueprintCallable, Category = "Lifes")
	int GetLifesLeft() const;

	// Starting Health Points to act. EditAnywhere to set individual Health Points
	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 StartingHealth = 100;

	// Current Health for this actor
	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth = 0;

	// Starting Life Count
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingLifes = 0;

	// Current Lifes left
	UPROPERTY(VisibleAnywhere, Category = "Lifes")
	int32 CurrentLifes = 0;
#pragma endregion Settings for Life and Health

};
