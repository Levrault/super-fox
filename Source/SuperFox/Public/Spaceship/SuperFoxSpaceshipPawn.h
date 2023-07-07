// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spaceship/BaseSpaceshipPawn.h"
#include "SuperFoxSpaceshipPawn.generated.h"

/**
 * 
 */
UCLASS()
class SUPERFOX_API ASuperFoxSpaceshipPawn : public ABaseSpaceshipPawn
{
	GENERATED_BODY()

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "50.0", ClampMax = "400.0"))
	float Speed = 100.f;

	UPROPERTY(EditAnywhere, Category = "Spline Track")
	class ASplineTrack* SplineTrackComponent;

	UPROPERTY(EditAnywhere, Category = "Spline Track", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CameraPoint;
};
