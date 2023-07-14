// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedActionKeyMapping.h"
#include "InputActionValue.h"
#include "SuperFoxSpaceshipPawn.generated.h"

UCLASS()
class SUPERFOX_API ASuperFoxSpaceshipPawn : public APawn
{
	GENERATED_BODY()

public:
	ASuperFoxSpaceshipPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Fire(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticBaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
	class UFloatingPawnMovement* SpaceshipMovement;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;

	float ViewportMaxX;
	float ViewportMaxY;
	FRotator InitialSpaceshipRotation;

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	UFloatingPawnMovement* GetSpaceshipMovement() const { return SpaceshipMovement;  }

	UFUNCTION()
	FVector2D GetWorldLocationToScreen() const;
	
	UFUNCTION()
	FVector LerpSpaceshipVelocityToZero() const;

	UFUNCTION()
	void SpaceshipTiltAnimation();

	// Enhanced Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputFire;

	UPROPERTY(EditAnywhere, Category = "Movements")
	float AxisMovementSpeed = 1200.f;
	
	UPROPERTY(EditAnywhere, Category = "Rail Settings", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float AxisDecelerationOnScreenEdges = 0.2f;

	// Rails
	UPROPERTY(EditAnywhere, Category = "Rail Settings")
	FVector2D TrackInset;

	UPROPERTY(EditAnywhere, Category = "Rail Settings")
	class ASplineTrack* SplineTrackComponent;

	UPROPERTY(EditAnywhere, Category = "Rail Settings", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CameraPoint;

	UPROPERTY(EditAnywhere, Category = "Rail Settings")
	float ForwardSpeed = 2000.f;

};
