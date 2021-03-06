// © 2018 YVES TANAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "FPMovementComponent.generated.h"

class UTrack;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TURRETPOWER_API UFPMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Initialize(UTrack* LeftTrackToSet, UTrack* RightTrackToSet);

	// calls both tracks to move forward
	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendToMoveForwad(float Throw);

	// Calls one track to move backwards and the other to move forward
	// Positive Values intend turn right, negative intend turn left
	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendTurnRight(float Throw);


private:
	// Called from the Pathfinding logic by the AI Controllers
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	UTrack* LeftTrack = nullptr;
	UTrack* RightTrack = nullptr;
};
