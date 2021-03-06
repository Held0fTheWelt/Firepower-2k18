// © 2018 YVES TANAS

#include "CannonProjectilePool.h"

#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "Public/TimerManager.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "OBJPool.h"
#include "TrackedMilitaryEntity.h"
#include "TrackedMilitaryVehicle.h"
#include "AimingComponent.h"
#include "Barrel.h"

#include "PooledCannonProjectile.h"
#define CURRENTTIMESTAMP GetWorld()->GetTimeSeconds()
#define FORWARDVECTOR FVector::ForwardVector
// Sets default values
ACannonProjectilePool::ACannonProjectilePool()
{
	UBoxComponent* PoolRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	PoolRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PoolRoot->SetCollisionObjectType(ECC_WorldStatic);
	RootComponent = PoolRoot;
	
	ObjectPool = CreateDefaultSubobject<UObjectPool>(TEXT("Pool Component"));
	ObjectPool->TemplateClass = ACannonProjectilePool::StaticClass();
	ObjectPool->AutoInitialize = true;
	ObjectPool->PoolSize = 1;
}

void ACannonProjectilePool::BeginPlay()
{
	Super::BeginPlay();

	// find all actors to listen to
	for (TActorIterator<ATrackedMilitaryEntity> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators. 
		ATrackedMilitaryEntity *Pawn = *ActorItr;
		UAimingComponent* AimingComponent = Pawn->FindComponentByClass<UAimingComponent>();
		if (AimingComponent)
		{
			AimingComponent->FireProjectile.AddDynamic(this, &ACannonProjectilePool::LaunchProjectile);
		}
	}

	for (TActorIterator<ATrackedMilitaryVehicle> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators. 
		ATrackedMilitaryVehicle *Pawn = *ActorItr;
		UAimingComponent* AimingComponent = Pawn->FindComponentByClass<UAimingComponent>();
		if (AimingComponent)
		{
			AimingComponent->FireProjectile.AddDynamic(this, &ACannonProjectilePool::LaunchProjectile);
		}
	}
}

void ACannonProjectilePool::LaunchProjectile(FVector Location, FRotator Rotation, float LaunchSpeed, int8 CannonType, FVector SpawnScale)
{
		//UE_LOG(LogTemp, Warning, TEXT("%f: Firing a round on Pool"), CURRENTTIMESTAMP)

	// if it's a cannon
	if (CannonType == 2)
		return;

	bool Result;
	FPoolSpawnOptions SpawnOptions;
	SpawnOptions.ActorTickEnabled = true;
	SpawnOptions.EnableCollision = true;
	SpawnOptions.SimulatePhysics = true;
	SpawnOptions.CollisionType = EPoolCollisionType::QueryAndPhysics;
	//
	FTransform SpawnTransform = FTransform(Rotation, Location, FVector(SpawnScale));
	//
	for (auto &BLT : ObjectPool->GetInactiveObjects<APooledCannonProjectile>()) {
		BLT->BulletSpeed = LaunchSpeed;
		#if WITH_EDITOR
		BLT->SetFolderPath("Projectiles/Cannon");
		#endif
	}
	// BeginDeferredSpawnFromPool(const UObject* WorldContextObject, UObjectPool* ObjectPool, UClass* PooledClass, const FPoolSpawnOptions &SpawnOptions, const FTransform &SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner, const bool Reconstruct, bool &SpawnSuccessful) {
	auto Projectile = ObjectPool->BeginDeferredSpawnFromPool(GetWorld(), ObjectPool, APooledCannonProjectile::StaticClass(), SpawnOptions, SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this, false, Result);
	auto Bullet = Cast<APooledCannonProjectile>(Projectile);
	if (Bullet) {
		ObjectPool->FinishDeferredSpawnFromPool(Bullet, false, SpawnTransform);
		//		UE_LOG(LogTemp, Warning, TEXT("{Pool}:: FIRED!"));
	}

}