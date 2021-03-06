// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WheelSpawnPoint.generated.h"

class ASuspensionWheel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TURRETPOWER_API UWheelSpawnPoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	UWheelSpawnPoint();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	ASuspensionWheel* GetSpawnedSuspensionWheel() const { return SpawnedActor; };
protected:
	virtual void BeginPlay() override;

private:
	// Configurable Spawn Point for Wheels
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ASuspensionWheel> SpawnClass = nullptr;

	void SetupConstraint();

	UPROPERTY()
	ASuspensionWheel* SpawnedActor = nullptr;
};
