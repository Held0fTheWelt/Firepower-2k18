// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Track.generated.h"

class UMaterialInstanceDynamic;
class USkeletalMeshComponent;
class USplineComponent;
class UStaticMeshComponent;
class ASuspensionWheel;

/** TankTrack is used to set maximum driving force and apply forces to the tank */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories = ("Mobile"))
class TURRETPOWER_API UTrack : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
public:

	UTrack();

	void Initialize(USplineComponent* Spline, TArray<FVector> SplinePoints, TArray<FVector> SplineTangents, float NumberOfTreads, TArray<FVector>* Locations, TArray<FVector>* Scales, USkeletalMeshComponent* Tread);

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float SplineScaleVisualityWidth = 30.f;

	UPROPERTY(EditAnywhere, Category = "Components")
	USplineComponent* Spline = nullptr;

	UMaterialInstanceDynamic* DynamicMaterial = nullptr;
		
	USkeletalMeshComponent* Tread = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void RegisterSuspensionHandlers(TArray<UStaticMeshComponent*> SuspensionHandles);
private:
	
	int TreadsLastIndex;

	void InitializeSpline(TArray<FVector> SplinePoints, TArray<FVector> SplineTangents);
	void BuildTreads(float NumberOfTreads);
	void CreateMaterialForSimpleTracks();

	TArray<UStaticMeshComponent*> SuspensionHandles;

public:
	// Sets a Throttle between -1 and 1
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetThrottle(float Throttle);

	///Assume a tank of 40000kg at 1g acceleration ín cm/s²
	// Max Force per Track in Newton
	UPROPERTY(EditDefaultsOnly)
	float MaxDrivingForce = 40000000;
	
	void ApplyForwardForce(float Throttle);
	TArray<ASuspensionWheel*> GetWheels() const;
};
