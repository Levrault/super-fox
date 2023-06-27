// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseSpaceshipPawn.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABaseSpaceshipPawn::ABaseSpaceshipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = StaticBaseMesh;
}

// Called when the game starts or when spawned
void ABaseSpaceshipPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseSpaceshipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseSpaceshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

