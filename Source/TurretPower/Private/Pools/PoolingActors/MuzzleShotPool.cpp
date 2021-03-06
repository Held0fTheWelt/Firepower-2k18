// © 2018 YVES TANAS

#include "MuzzleShotPool.h"
#include "OBJPool.h"

#include "EngineUtils.h"
#include "Public/TimerManager.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Components/BoxComponent.h"
#include "TrackedMilitaryEntity.h"
#include "AimingComponent.h"
#include "Barrel.h"

#include "PooledExplosion.h"

#define CURRENTTIMESTAMP GetWorld()->GetTimeSeconds()

// Sets default values
AMuzzleShotPool::AMuzzleShotPool()
{
	UBoxComponent* PoolRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	PoolRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PoolRoot->SetCollisionObjectType(ECC_WorldStatic);
	RootComponent = PoolRoot;

	ObjectPool = CreateDefaultSubobject<USharedObjectPool>(TEXT("Pool Component"));
	ObjectPool->AutoInitialize = true;
}

void AMuzzleShotPool::BeginPlay()
{
	Super::BeginPlay();

	// find all actors to listen to
	for (TActorIterator<ATrackedMilitaryEntity> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators. 
		ATrackedMilitaryEntity *Pawn = *ActorItr;
		UAimingComponent* AimingComponent = Pawn->FindComponentByClass<UAimingComponent>();
		if (AimingComponent)
		{
			AimingComponent->FireProjectile.AddDynamic(this, &AMuzzleShotPool::LaunchProjectile);
		}
	}
}

void AMuzzleShotPool::LaunchProjectile(FVector Location, FRotator Rotation, float LaunchSpeed, int8 CannonType, FVector SpawnScale)
{
	UClass* currentClass = nullptr;

	if (CannonType == 0)
		currentClass = Cannon;
	if (CannonType == 1)
		currentClass = Gun;
	if (CannonType == 2)
		currentClass = Missile;
	bool Result;
	FPoolSpawnOptions SpawnOptions;
	SpawnOptions.ActorTickEnabled = true;
	SpawnOptions.EnableCollision = true;
	SpawnOptions.SimulatePhysics = true;
	SpawnOptions.CollisionType = EPoolCollisionType::QueryAndPhysics;

	FTransform SpawnTransform = FTransform(Rotation, Location, FVector(1,1,1));

	for (auto &BLT : ObjectPool->GetInactiveObjects<APooledExplosion>()) {
		#if WITH_EDITOR
		BLT->SetFolderPath("Projectiles/MuzzleShot");
		#endif
	}

	auto Projectile = ObjectPool->BeginDeferredSpawnFromPool(this, ObjectPool, currentClass, SpawnOptions, SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this, false, Result);
	auto Bullet = Cast<APooledExplosion>(Projectile);
	if (Bullet) {
		ObjectPool->FinishDeferredSpawnFromPool(Bullet, false, SpawnTransform);
	}
	
}