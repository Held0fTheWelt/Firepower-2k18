// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BarrelInterface.h"
#include "AbstractBarrel.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TURRETPOWER_API UAbstractBarrel : public UActorComponent, public IBarrelInterface
{
	GENERATED_BODY()
public:
	IBarrelInterface* GetBarrel() {
		return this;
	};
};
