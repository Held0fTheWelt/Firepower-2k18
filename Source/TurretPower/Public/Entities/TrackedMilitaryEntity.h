// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrackedMilitaryEntity.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurretDeathDelegate);

class UArrowComponent;
class UAimingComponent;
class UAudioEffectComponent;
class UProjectileObjectPoolComponent;
class UTurret;
class UBarrel;

UCLASS()
class TURRETPOWER_API ATrackedMilitaryEntity : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrackedMilitaryEntity();

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnBarrelFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnDestruction();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	AActor* GetLockedOnTarget();

	void SetLockedOnTarget(AActor* Target);
	
	// Call this in Blueprint inside the Construction Script to make Composition possible
	UFUNCTION(BlueprintCallable, Category = "Setup")
	virtual void Initialize();
		
	UFUNCTION(BlueprintCallable, Category = "Setup")
	UBarrel* GetPrimaryBarrel();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	UTurret* GetPrimaryTurret();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	UAimingComponent* GetAimingComponent();
	
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

	AActor* LockedOnTarget;
private:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Body = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTurret* PrimaryTurret = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBarrel* PrimaryBarrel = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTurret* SecondaryTurret = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBarrel* SecondaryBarrel = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArrowComponent* PrimaryDirection = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArrowComponent* SecondaryDirection = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAimingComponent* AimingComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioEffectComponent* AudioComponent = nullptr;

public:

	FTurretDeathDelegate OnDeath;

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
};
