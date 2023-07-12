// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCamera.h"
#include "Level/SplineTrack.h"
#include "Components/SplineComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Spaceship/SuperFoxSpaceshipPawn.h"

// Sets default values
APlayerCamera::APlayerCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineTrackComponent = CreateOptionalDefaultSubobject<ASplineTrack>(TEXT("Spline Track"));
}

// Called when the game starts or when spawned
void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	// Set as default camera for the current pawn
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0)) {
		playerController->SetViewTarget(this);
		PlayerPawn = Cast<ASuperFoxSpaceshipPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	}

	if (SplineTrackComponent) {
		SplineTrackComponent->SnapCameraToSpline(this, CameraPoint);
	}
}

// Called every frame
void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn) {
		// TODO: Replace InterpSpeed, Calcule de la distance par defaut
		SplineTrackComponent->FollowTargetPawnAlongSpline(PlayerPawn, this, PlayerPawn->ForwardSpeed, FVector(0, -3000, 0));
	}

}

void APlayerCamera::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(APlayerCamera, SplineTrackComponent)) {
		if (SplineTrackComponent) {
			UE_LOG(LogTemp, Warning, TEXT("Spline Track Component changed, snapping to new spline"));
			SplineTrackComponent->SnapCameraToSpline(this, CameraPoint);
		}
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(APlayerCamera, CameraPoint)) {
		UE_LOG(LogTemp, Warning, TEXT("Spline Track Component changed, snapping to new spline"));
		if (SplineTrackComponent) {
			SplineTrackComponent->SnapCameraToSpline(this, CameraPoint);
		}
	}
}
