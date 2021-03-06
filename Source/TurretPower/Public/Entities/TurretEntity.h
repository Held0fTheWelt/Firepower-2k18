// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "MilitaryEntity.h"
#include "TurretEntity.generated.h"

class UBarrel;



UCLASS()
class TURRETPOWER_API ATurretEntity : public AMilitaryEntity
{
	GENERATED_BODY()
	
public:

	ATurretEntity();
	
	virtual void Initialize() override;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	UBarrel* GetPrimaryBarrel();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBarrel* PrimaryBarrel = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBarrel* SecondaryBarrel = nullptr;
};
