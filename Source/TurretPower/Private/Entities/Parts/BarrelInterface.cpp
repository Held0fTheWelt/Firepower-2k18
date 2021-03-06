// © 2018 YVES TANAS

#include "BarrelInterface.h"

EBarrelType IBarrelInterface::GetBarrelType() const{
	return EBarrelType::None;
}

void IBarrelInterface::Elevate(float RelativeSpeed)
{
	return;
}

FVector IBarrelInterface::GetMyForwardVector() const
{
	return FVector::ZeroVector;
}

FVector IBarrelInterface::GetMySocketLocation(FName name) const
{
	return FVector::ZeroVector;
}

FRotator IBarrelInterface::GetMySocketRotation(FName name) const
{
	return FRotator::ZeroRotator;
}
