// © 2018 YVES TANAS

#include "Track.h"

#include <iostream>

#include "ConstructorHelpers.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "SuspensionWheel.h"
#include "WheelSpawnPoint.h"

UTrack::UTrack()
{	
}

void UTrack::Initialize(USplineComponent* spline, TArray<FVector> SplinePoints, TArray<FVector> SplineTangents, float NumberOfTreads, TArray<FVector>* Locations, TArray<FVector>* Scales, USkeletalMeshComponent* tread)
{
	NumberOfTreads--;
	TreadsLastIndex = truncf(NumberOfTreads);
	Spline = spline;
	Tread = tread;
	
	Spline->SetClosedLoop(true);
	Spline->SetDrawDebug(true);

	InitializeSpline(SplinePoints, SplineTangents);
	BuildTreads(NumberOfTreads);

	//CreateMaterialForSimpleTracks();
}

void UTrack::InitializeSpline(TArray<FVector> SplinePoints, TArray<FVector> SplineTangents)
{
	Spline->SetSplineLocalPoints(SplinePoints);

	int32 index = 0;
	for (auto tangent : SplineTangents)
	{
		Spline->SetTangentAtSplinePoint(index, tangent, ESplineCoordinateSpace::Local, false);
		index++;
	}

	Spline->UpdateSpline();
}

void UTrack::BuildTreads(float NumberOfTreads)
{
	for (int32 i = 0; i < GetInstanceCount(); i++)
	{
		RemoveInstance(i);
	}
	
	for (int32 i = 0; i < TreadsLastIndex; i++)
	{
		float CurrentTreadLength = Spline->GetSplineLength() / NumberOfTreads * i;

		FVector Location = Spline->GetLocationAtDistanceAlongSpline(CurrentTreadLength, ESplineCoordinateSpace::Local);
		FRotator Rotation = Spline->GetRotationAtDistanceAlongSpline(CurrentTreadLength, ESplineCoordinateSpace::Local);
		float RightIdentifier = Spline->GetRightVectorAtDistanceAlongSpline(CurrentTreadLength, ESplineCoordinateSpace::Local).Y;

		if (RightIdentifier < 0)
		{
			Rotation.Roll = 180;
		}

		FTransform transform;
		transform.SetLocation(Location);
		transform.SetRotation(Rotation.Quaternion());

		AddInstance(transform);
	}
}

void UTrack::CreateMaterialForSimpleTracks()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialInstanceBlueprint(TEXT("/Game/MilitaryVehicles/Materials/MI_Tread_Inst.MI_Tread_Inst"));
	if (MaterialInstanceBlueprint.Succeeded())
	{
		DynamicMaterial = CreateDynamicMaterialInstance(0, MaterialInstanceBlueprint.Object);
		Tread->SetMaterial(0,DynamicMaterial);
	}
}

void UTrack::SetThrottle(float Throttle)
{
	// add the new _Throttle to the current throttle, but keep the values clamped between -1 and 1
	ApplyForwardForce(FMath::Clamp<float>(Throttle, -1, 1));
}

void UTrack::ApplyForwardForce(float Throttle)
{
	auto ForceApplied = Throttle * MaxDrivingForce;

	auto Wheels = GetWheels();

	auto ForcePerWheel = ForceApplied / Wheels.Num();

	for (ASuspensionWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}

TArray<ASuspensionWheel*> UTrack::GetWheels() const
{
	// create a result array
	TArray<ASuspensionWheel*> ResultArray;
	// create an array for all children on this node
	TArray <USceneComponent*> Children;
	// Get all children of USceneComponent
	GetChildrenComponents(true, Children);
	// 
	for (USceneComponent* Child : Children)
	{
		auto SpawnPointChild = Cast<UWheelSpawnPoint>(Child);

		if (!SpawnPointChild)
			continue;

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedSuspensionWheel();
		if (!SpawnedChild)
			continue;

		auto SprungWheel = Cast<ASuspensionWheel>(SpawnedChild);
		if (!SprungWheel)
			continue;
		ResultArray.Add(SprungWheel);
	}


	// Return the Result Array
	return ResultArray;
}

void UTrack::RegisterSuspensionHandlers(TArray<UStaticMeshComponent*> suspensionHandles)
{
	SuspensionHandles = suspensionHandles;
}
