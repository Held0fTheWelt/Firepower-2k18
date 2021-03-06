// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CannonProjectilePool.generated.h"

class UObjectPool;

UCLASS()
class TURRETPOWER_API ACannonProjectilePool : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACannonProjectilePool();


protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UObjectPool* ObjectPool;

	// Called when the game starts or when created
	virtual void BeginPlay() override;

	UFUNCTION()
	void LaunchProjectile(FVector Location, FRotator Rotation, float LaunchSpeed, int8 CannonType, FVector SpawnScale);
};
