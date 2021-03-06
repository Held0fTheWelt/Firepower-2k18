// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetHitExplosionPool.generated.h"

class UObjectPool;

UCLASS()
class TURRETPOWER_API ATargetHitExplosionPool : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ATargetHitExplosionPool();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FVector SpawnScale = FVector(1, 1, 1);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UObjectPool* ObjectPool;

	// Called when the game starts or when created
	virtual void BeginPlay() override;

	UFUNCTION()
	void LaunchProjectile(FVector Location);
};
