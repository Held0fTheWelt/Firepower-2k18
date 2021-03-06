// © 2018 YVES TANAS

#include "PooledCannonProjectile.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"


#include "ConstructorHelpers.h"
#include "PooledProjectile.h"
#include "AudioEffectComponent.h"

#define CURRENTTIMESTAMP GetWorld()->GetTimeSeconds()
// Sets default values
APooledCannonProjectile::APooledCannonProjectile()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	SetRootComponent(ProjectileMesh);	

	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetEnableGravity(true);
	ProjectileMesh->SetNotifyRigidBodyCollision(true);
	ProjectileMesh->SetCollisionProfileName(FName("Projectile"));
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileMesh->OnComponentHit.AddDynamic(this, &APooledCannonProjectile::OnHit);

	OnPoolBeginPlay.AddDynamic(this, &APooledCannonProjectile::Shoot);
	OnPoolEndPlay.AddDynamic(this, &APooledCannonProjectile::Break);
}

#define FORWARDVECTOR FVector::ForwardVector
void APooledCannonProjectile::OnTimerExpire()
{
	ReturnToPool();
}

UStaticMeshComponent * APooledCannonProjectile::GetMyProjectileMesh() const
{
	return ProjectileMesh;
}

void APooledCannonProjectile::Break_Implementation() {

}

void APooledCannonProjectile::Shoot_Implementation() {

	if (ReturnDelay != 0.0f)
	{
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &APooledCannonProjectile::OnTimerExpire, ReturnDelay, false);
	}
}

void APooledCannonProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult &Hit)
{
	if (!OtherActor->IsValidLowLevelFast() || !OtherComponent->IsValidLowLevelFast()) { ReturnToPool(); return; }
	//
	if (OtherActor != this) {

		UGameplayStatics::ApplyRadialDamageWithFalloff(this,
			DamageDefaultAmount,
			DamageMinimumDefaultAmount,
			GetActorLocation(),
			100, // Make it one meter inner Radius
			Radius,
			DamageFallOff,
			UDamageType::StaticClass(),
			TArray<AActor*>() // damage all actors
		);

		OtherComponent->AddImpulseAtLocation(GetVelocity() * 20, GetActorLocation(), NAME_None);
		
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("HIT! : %s"), *OtherActor->GetName()));
		Explode.Broadcast(GetActorLocation());// Hit.Location);
	}

	ReturnToPool();
}