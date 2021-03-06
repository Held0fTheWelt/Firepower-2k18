// © 2018 YVES TANAS

#include "SkeletalBarrel.h"


#include "Engine/World.h"

// move barrel the right amount this frame
// Given a max elevation speed and the frame time
void USkeletalBarrel::Elevate(float RelativeSpeed)
{
	// Clamp Relative Speed to maximum possible values between -1 and 1
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);

	// Calculate the ElevationChange by multiplying the RelativeSpeed with MaxDegreesPerSeconds and the given amount of DeltaTime for this frame
	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	// The RawNewElevation is our relative rotations pitch, added by the calculated amount of ElevationChange
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	// Clamp the elevation to the given min and max values for elevation degrees
	auto Elevation = FMath::Clamp(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
	// set the new relative rotation
	SetRelativeRotation(FRotator(Elevation, 0, RelativeRotation.Roll));
}

EBarrelType USkeletalBarrel::GetBarrelType() const
{
	return BarrelType;
}

FVector USkeletalBarrel::GetMyForwardVector() const
{
	return GetForwardVector();
}

FVector USkeletalBarrel::GetMySocketLocation(FName name) const
{
	return GetSocketLocation(name);
}

FRotator USkeletalBarrel::GetMySocketRotation(FName name) const
{
	return GetSocketRotation(name);
}
