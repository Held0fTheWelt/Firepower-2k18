// © 2018 YVES TANAS

#include "FPMovementComponent.h"

#include "GameFramework/Actor.h"

#include "Track.h"

#define CURRENTTIMESTAMP GetWorld()->GetTimeSeconds()

void UFPMovementComponent::Initialize(UTrack* LeftTrackToSet, UTrack* RightTrackToSet) {

	if (!LeftTrackToSet || !RightTrackToSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: Tracks not set ordinary on %s"), CURRENTTIMESTAMP, *GetName())
		return;
	}

	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

// calls both tracks to move forward
void UFPMovementComponent::IntendToMoveForwad(float Throw)
{
	if (Throw == 0)
		return;

	if (!(LeftTrack && RightTrack))
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: Tracks not set ordinary on %s"), CURRENTTIMESTAMP, *GetName())
		return;
	}
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

// calls both tracks to turn right
void UFPMovementComponent::IntendTurnRight(float Throw)
{
	if (Throw == 0)
		return;
	
	if (!(LeftTrack && RightTrack))
	{
		UE_LOG(LogTemp, Warning, TEXT("%f: Tracks not set ordinary on %s"), CURRENTTIMESTAMP, *GetName())
		return;
	}

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);


}

void UFPMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{	
	UE_LOG(LogTemp, Warning, TEXT("%f: Moving to Player"), CURRENTTIMESTAMP)
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	IntendToMoveForwad(ForwardThrow);

	auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
	IntendTurnRight(RightThrow);
}