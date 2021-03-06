// © 2018 YVES TANAS

#include "AudioEffectComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UAudioEffectComponent::UAudioEffectComponent() { PrimaryComponentTick.bCanEverTick = false; }

void UAudioEffectComponent::PlayAudioEffect()
{
	if (!GetOwner()) return;
	if (!AudioEffect) return;

	UGameplayStatics::PlaySoundAtLocation(this, AudioEffect, GetOwner()->GetActorLocation());// , Volume, Pitch);
}
