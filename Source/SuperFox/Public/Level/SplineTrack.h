// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineTrack.generated.h"

UCLASS()
class SUPERFOX_API ASplineTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineTrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Spline Track")
	class USplineComponent* SplineTrackComponent;

	float GetDistanceAlongSpline(float CameraPoint);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USplineComponent* GetSplineTrackComponent() const { return SplineTrackComponent; }

	UFUNCTION()
	void SnapCameraToSpline(class APlayerCamera* PlayerCamera, float CameraPoint);

	UFUNCTION()
	void SnapPawnToSpline(class APawn* PlayerPawn, float CameraPoint);
};
