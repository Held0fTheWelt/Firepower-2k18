// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"
#include "PooledExplosion.generated.h"

class UParticleSystemComponent;
class USoundCue;
class UAudioComponent;

UCLASS()
class TURRETPOWER_API APooledExplosion : public APooledActor
{
	GENERATED_BODY()

	APooledExplosion();
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* ParticleSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USoundCue* Sound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReturnDelay = 1.f;
public:
	/// Manually fire this Projectile, instead of relying on Component Initialization.
	UFUNCTION(Category = "Projectile", BlueprintNativeEvent, meta = (DisplayName = "Shoot", Keywords = "shoot projectile"))
	void Shoot(); virtual void Shoot_Implementation();

	/// Manually stops this Projectile, instead of relying on Component Destruction.
	UFUNCTION(Category = "Projectile", BlueprintNativeEvent, meta = (DisplayName = "Break", Keywords = "break projectile"))
	void Break(); virtual void Break_Implementation();

private:
	UFUNCTION()
	void OnTimerExpire();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float Volume = .75f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float Pitch = 1.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
