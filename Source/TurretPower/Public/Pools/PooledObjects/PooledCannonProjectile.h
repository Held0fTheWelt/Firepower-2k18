// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"
#include "PooledCannonProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileHit, FVector, Location);

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UAudioEffectComponent;

UCLASS()
class TURRETPOWER_API APooledCannonProjectile : public APooledActor
{
	GENERATED_BODY()
	//
	APooledCannonProjectile();

private:

	UFUNCTION()
	void OnTimerExpire();
	

public:
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (DisplayName = "Projectile Mesh"))
	UStaticMeshComponent* ProjectileMesh;
	
	UFUNCTION(BlueprintCallable, Category = "Firing")
	UStaticMeshComponent* GetMyProjectileMesh() const;
	UPROPERTY()
	FOnProjectileHit Explode;

	// Pointer to the projectiles movement component.
	UPROPERTY(BlueprintReadWrite, Category = "Components", meta = (DisplayName = "Projectile Mesh"))
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

//	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Projectile Component"))
//	UPooledProjectile* ProjectilePhyX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BulletSpeed = 3000;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReturnDelay = 5.f;

	/// Manually fire this Projectile, instead of relying on Component Initialization.
	UFUNCTION(Category = "Projectile", BlueprintNativeEvent, meta = (DisplayName = "Shoot", Keywords = "shoot projectile"))
	void Shoot(); virtual void Shoot_Implementation();

	/// Manually stops this Projectile, instead of relying on Component Destruction.
	UFUNCTION(Category = "Projectile", BlueprintNativeEvent, meta = (DisplayName = "Break", Keywords = "break projectile"))
	void Break(); virtual void Break_Implementation();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float Radius = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DamageDefaultAmount = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DamageFallOff = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DamageMinimumDefaultAmount = 2.0f;
};
