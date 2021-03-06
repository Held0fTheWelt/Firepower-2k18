// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AudioEffectComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TURRETPOWER_API UAudioEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAudioEffectComponent();

	void PlayAudioEffect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* AudioEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float Volume = .75f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float Pitch = 1.f;
};
