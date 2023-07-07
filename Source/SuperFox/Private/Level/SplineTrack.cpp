// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/SplineTrack.h"
#include "Components/SplineComponent.h"
#include "Player/PlayerCamera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASplineTrack::ASplineTrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineTrackComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Track"));
	RootComponent = SplineTrackComponent;
}

// Called when the game starts or when spawned
void ASplineTrack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplineTrack::SnapCameraToSpline(APlayerCamera* PlayerCamera, float CameraPoint)
{
	if (PlayerCamera == nullptr) {
		return;
	}

	float distance = GetDistanceAlongSpline(CameraPoint);

	FVector locationAlongSline = SplineTrackComponent->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	FRotator rotationAlongSpline = SplineTrackComponent->GetRotationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);

	UCameraComponent* cameraComponent = PlayerCamera->GetCameraComponent();
	cameraComponent->SetWorldRotation(rotationAlongSpline);
	cameraComponent->SetWorldLocation(locationAlongSline);
}

void ASplineTrack::SnapPawnToSpline(APawn* PlayerPawn, float CameraPoint)
{
	if (PlayerPawn == nullptr) {
		return;
	}

	float distance = GetDistanceAlongSpline(CameraPoint);

	FVector locationAlongSline = SplineTrackComponent->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	FRotator rotationAlongSpline = SplineTrackComponent->GetRotationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	
	PlayerPawn->GetRootComponent()->SetWorldRotation(rotationAlongSpline);
	PlayerPawn->GetRootComponent()->SetWorldLocation(locationAlongSline);
}


void ASplineTrack::FollowTargetPawnAlongSpline(APawn* PlayerPawn, APlayerCamera* Camera, float InterpSpeed, FVector CameraOffset) {
	if (PlayerPawn == nullptr) {
		return;
	}
	if (Camera == nullptr) {
		return;
	}

	FVector playerPositionAlongSpline = SplineTrackComponent->FindLocationClosestToWorldLocation(PlayerPawn->GetActorLocation(), ESplineCoordinateSpace::World);

	auto positionAlongSpline = FMath::VInterpTo(
		Camera->GetCameraComponent()->GetComponentLocation(),
		playerPositionAlongSpline,
		UGameplayStatics::GetWorldDeltaSeconds(this),
		InterpSpeed
	);
	Camera->GetCameraComponent()->SetWorldLocation(positionAlongSpline - CameraOffset);
}

float ASplineTrack::GetDistanceAlongSpline(float CameraPoint)
{
	return FMath::Lerp(0, SplineTrackComponent->GetSplineLength(), CameraPoint);
}
