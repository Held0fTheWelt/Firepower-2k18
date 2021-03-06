// © 2018 YVES TANAS

#include "TurretEntity.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

#include "AimingComponent.h"
#include "AudioEffectComponent.h"
#include "Turret.h"

ATurretEntity::ATurretEntity()
{
	PrimaryActorTick.bCanEverTick = true;

	Initialize();
}

void ATurretEntity::Initialize()
{
	Super::Initialize();


}

UBarrel * ATurretEntity::GetPrimaryBarrel()
{
	return PrimaryBarrel;
}

