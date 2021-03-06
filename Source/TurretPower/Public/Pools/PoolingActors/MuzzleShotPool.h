// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuzzleShotPool.generated.h"

class USharedObjectPool;

UCLASS()
class TURRETPOWER_API AMuzzleShotPool : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMuzzleShotPool();

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USharedObjectPool* ObjectPool;

	// Called when the game starts or when created
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UClass* Cannon;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UClass* Gun;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UClass* Missile;

	UFUNCTION()
	void LaunchProjectile(FVector Location, FRotator Rotation, float LaunchSpeed, int8 CannonType, FVector SpawnScale);
};
