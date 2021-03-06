// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuspensionWheel.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPhysicsConstraintComponent;

UCLASS()
class TURRETPOWER_API ASuspensionWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuspensionWheel();

	virtual void Tick(float DeltaTime) override;

	void AddDrivingForce(float ForceMagnitudeToApply);
	

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* AxelWheelConstraint = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SprungAxel = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SprungWheel = nullptr;

	virtual void BeginPlay() override;

	void ApplyForce();
	void ResetForce();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetupConstraint();

	float TotalForceMagnitudeThisFrame = 0;
};
