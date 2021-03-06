// © 2018 YVES TANAS

#include "TargetHitExplosionPool.h"
#include "OBJPool.h"

#include "EngineUtils.h"
#include "Public/TimerManager.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Components/BoxComponent.h"
#include "PooledCannonProjectile.h"
#include "AimingComponent.h"

#include "PooledExplosion.h"

#define CURRENTTIMESTAMP GetWorld()->GetTimeSeconds()

// Sets default values
ATargetHitExplosionPool::ATargetHitExplosionPool()
{	
	UBoxComponent* PoolRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	PoolRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PoolRoot->SetCollisionObjectType(ECC_WorldStatic);
	RootComponent = PoolRoot;

	ObjectPool = CreateDefaultSubobject<UObjectPool>(TEXT("Pool Component"));
	ObjectPool->TemplateClass = ATargetHitExplosionPool::StaticClass();
	ObjectPool->AutoInitialize = true;
	ObjectPool->PoolSize = 1;
}

void ATargetHitExplosionPool::BeginPlay()
{
	Super::BeginPlay();

	// find all actors to listen to
	for (TActorIterator<APooledCannonProjectile> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators. 
		APooledCannonProjectile *Actor = *ActorItr;
			
		Actor->Explode.AddDynamic(this, &ATargetHitExplosionPool::LaunchProjectile);
	}
}

void ATargetHitExplosionPool::LaunchProjectile(FVector Location)
{
	bool Result;
	FPoolSpawnOptions SpawnOptions;
	SpawnOptions.ActorTickEnabled = true;
	SpawnOptions.EnableCollision = true;
	SpawnOptions.SimulatePhysics = true;
	SpawnOptions.CollisionType = EPoolCollisionType::QueryAndPhysics;

	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, Location, FVector(SpawnScale));

	for (auto &BLT : ObjectPool->GetInactiveObjects<APooledExplosion>()) {
		#if WITH_EDITOR
		BLT->SetFolderPath("Projectiles/TargetHit");
		#endif
	}

	auto Projectile = ObjectPool->BeginDeferredSpawnFromPool(GetWorld(), ObjectPool, APooledCannonProjectile::StaticClass(),  SpawnOptions, SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this, false, Result);
	auto Bullet = Cast<APooledExplosion>(Projectile);
	if (Bullet) {
		ObjectPool->FinishDeferredSpawnFromPool(Bullet, false, SpawnTransform);
	}
}