// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/SplineTrack.h"
#include "Components/SplineComponent.h"


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

