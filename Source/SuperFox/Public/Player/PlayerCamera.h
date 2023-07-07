// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "PlayerCamera.generated.h"

UCLASS()
class SUPERFOX_API APlayerCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere, Category = "Spline Track")
	class ASplineTrack* SplineTrackComponent;

	UPROPERTY(EditAnywhere, Category = "Spline Track", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CameraPoint;

private:
	APawn* playerPawn;
};
