// © 2018 YVES TANAS

#include "WheelSpawnPoint.h"
#include "SuspensionWheel.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWheelSpawnPoint::UWheelSpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWheelSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	SetupConstraint();
}


// Called every frame
void UWheelSpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWheelSpawnPoint::SetupConstraint()
{
	// Spawn a new Actor
	SpawnedActor = GetWorld()->SpawnActorDeferred<ASuspensionWheel>(SpawnClass, GetComponentTransform());
	// Protect for null reference
	if (!SpawnedActor) return;

	SpawnedActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	UGameplayStatics::FinishSpawningActor(SpawnedActor, GetComponentTransform());
}