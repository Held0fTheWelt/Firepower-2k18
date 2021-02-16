// © 2018 YVES TANAS

#include "Turret.h"

#include "Engine/World.h"

// rotate turret the right amount this frame
// Given a max rotation speed and the frame time
void UTurret::Rotate(float RelativeSpeed)
{
	// Clamp Relative Speed to maximum possible values between -1 and 1
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);

	// Calculate the RotationChange by multiplying the RelativeSpeed with MaxDegreesPerSeconds and the given amount of DeltaTime for this frame
	auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	// The RawNewRotation is our relative rotations pitch, added by the calculated amount of RotationChange
	auto Rotation = RelativeRotation.Yaw + RotationChange;
	// set the new relative rotation
	SetRelativeRotation(FRotator(0, Rotation, 0));
}