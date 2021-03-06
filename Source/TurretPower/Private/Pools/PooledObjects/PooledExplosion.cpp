// © 2018 YVES TANAS

#include "PooledExplosion.h"

#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

APooledExplosion::APooledExplosion() {
	if (!ParticleSystem)
		// Create a new Subobject of type UParticleSystemComponent to show a particle system
		ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("Particle Effect System");

	// Attach it to the RootComponent
	SetRootComponent(ParticleSystem);

	// Create a new Subobject of type UParticleSystemComponent to show a particle system
	Sound = CreateDefaultSubobject<USoundCue>("Sound Effect");

	// Create an audio component, the audio component wraps the Cue, 
	// and allows us to ineract with
	// it, and its parameters from code.
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(
		TEXT("AudioComponent")
		);

	// I don't want the sound playing the moment it's created.
	AudioComponent->bAutoActivate = false;
	
	OnPoolBeginPlay.AddDynamic(this, &APooledExplosion::Shoot);
	OnPoolEndPlay.AddDynamic(this, &APooledExplosion::Break);
}

void APooledExplosion::BeginPlay()
{
	Super::BeginPlay();


	if (AudioComponent && AudioComponent->IsValidLowLevelFast()) {
		AudioComponent->SetSound(Sound);
	}
}

void APooledExplosion::OnTimerExpire()
{
	if(AudioComponent)
		AudioComponent->Stop();
	ReturnToPool();
}

void APooledExplosion::Shoot_Implementation() {

	if(ParticleSystem)
		ParticleSystem->Activate(true);
	if(AudioComponent && !AudioComponent->IsPlaying())
		AudioComponent->Play();

	if (ReturnDelay != 0.0f)
	{
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &APooledExplosion::OnTimerExpire, ReturnDelay, false);
	}
}

void APooledExplosion::Break_Implementation() {
	ParticleSystem->Activate(false);
	
}