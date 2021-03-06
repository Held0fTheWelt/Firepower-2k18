// © 2018 YVES TANAS

#include "SuspensionWheel.h"

#include "Engine/World.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
ASuspensionWheel::ASuspensionWheel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;


	// Create a new Subobject of type UPhysicsConstraintComponent to use as Constraint
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
	// set this collision mesh the root component
	SetRootComponent(PhysicsConstraint);

	// Create a new Subobject of type UStaticMeshComponent to simulate wheel sprung
	SprungAxel = CreateDefaultSubobject<USphereComponent>(FName("Axel"));
	// Attach it to the Mass
	SprungAxel->SetupAttachment(PhysicsConstraint);

	// Create a new Subobject of type UStaticMeshComponent to simulate wheel sprung
	SprungWheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	// Attach it to the Mass
	SprungWheel->SetupAttachment(SprungAxel);

	// Create a new Subobject of type UPhysicsConstraintComponent to use as Constraint
	AxelWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxelWheelConstraint"));
	// set this collision mesh the root component
	AxelWheelConstraint->SetupAttachment(SprungAxel);
}

void ASuspensionWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	if (GetWorld()->TickGroup == TG_PostPhysics)
		ResetForce();
}

void ASuspensionWheel::AddDrivingForce(float ForceMagnitudeToApply)
{
	TotalForceMagnitudeThisFrame += ForceMagnitudeToApply;
}

void ASuspensionWheel::BeginPlay()
{
	Super::BeginPlay();

	SetupConstraint();

	if (!SprungWheel) return;
	SprungWheel->SetNotifyRigidBodyCollision(true);
	SprungWheel->OnComponentHit.AddDynamic(this, &ASuspensionWheel::OnHit);


}

void ASuspensionWheel::ApplyForce()
{
	SprungWheel->AddForce(SprungAxel->GetForwardVector() * TotalForceMagnitudeThisFrame);

}

void ASuspensionWheel::ResetForce()
{
	TotalForceMagnitudeThisFrame = 0.f;
}

void ASuspensionWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ApplyForce();
}

void ASuspensionWheel::SetupConstraint() {
	if (!GetAttachParentActor())	return;

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	if (!BodyRoot) return;

	PhysicsConstraint->SetConstrainedComponents(BodyRoot, NAME_None, SprungAxel, NAME_None);
	AxelWheelConstraint->SetConstrainedComponents(SprungAxel, NAME_None, SprungWheel, NAME_None);
}
