// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCamera.h"
#include "Level/SplineTrack.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Components/SplineComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCamera::APlayerCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateOptionalDefaultSubobject<ASplineTrack>(TEXT("Spline Track"));
}

// Called when the game starts or when spawned
void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCamera::SnapToSpline()
{
	if (SplineComponent == nullptr) {
		return;
	}

	USplineComponent* splineTrack = SplineComponent->GetSplineTrackComponent();
	float splineLenght = splineTrack->GetSplineLength();
	
	float distance = FMath::Lerp(0, splineLenght, CameraPoint);

	FVector locationAlongSline = splineTrack->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	FRotator rotationAlongSpline = splineTrack->GetRotationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	
	UCameraComponent* cameraComponent = GetCameraComponent();
	cameraComponent->SetWorldRotation(rotationAlongSpline);
	cameraComponent->SetWorldLocation(locationAlongSline);
}