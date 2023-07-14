#include "Spaceship/SuperFoxSpaceshipPawn.h"
#include "Level/SplineTrack.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"


ASuperFoxSpaceshipPawn::ASuperFoxSpaceshipPawn()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;
	
	StaticBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	StaticBaseMesh->SetupAttachment(RootComponent);

	SpaceshipMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
	SpaceshipMovement->SetUpdatedComponent(StaticBaseMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

void ASuperFoxSpaceshipPawn::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASuperFoxSpaceshipPawn, SplineTrackComponent)) {
		if (SplineTrackComponent) {
			UE_LOG(LogTemp, Warning, TEXT("Spline Track Component changed, snapping to new spline"));
			SplineTrackComponent->SnapPawnToSpline(this, CameraPoint);
		}
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASuperFoxSpaceshipPawn, CameraPoint)) {
		UE_LOG(LogTemp, Warning, TEXT("Spline Track Component changed, snapping to new spline"));
		if (SplineTrackComponent) {
			SplineTrackComponent->SnapPawnToSpline(this, CameraPoint);
		}
	}
}

// Called when the game starts or when spawned
void ASuperFoxSpaceshipPawn::BeginPlay()
{
	Super::BeginPlay();

	// cache screen size
	APlayerController* playerController = Cast<APlayerController>(GetController());
	int32 originalViewportWidth, originalViewportHeight;
	playerController->GetViewportSize(originalViewportWidth, originalViewportHeight);
	ViewportMaxX = static_cast<float>(originalViewportWidth) - TrackInset.X;
	ViewportMaxY = static_cast<float>(originalViewportHeight) - TrackInset.Y;
	
	// params
	SpaceshipMovement->MaxSpeed = AxisMovementSpeed;
	InitialSpaceshipRotation = StaticBaseMesh->GetComponentRotation();

	if (SplineTrackComponent) {
		SplineTrackComponent->SnapPawnToSpline(this, CameraPoint);
	}

}

// Called every frame
void ASuperFoxSpaceshipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	if (SplineTrackComponent) {
		CameraPoint += DeltaTime * 0.05f;
		SplineTrackComponent->SnapPawnToSpline(this, CameraPoint);
	}

	SpaceshipTiltAnimation();
}

// Called to bind functionality to input
void ASuperFoxSpaceshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* uiSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	
	uiSubSystem->AddMappingContext(DefaultMappingContext, 0);

	if (UEnhancedInputComponent* playerEIcomponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		playerEIcomponent->BindAction(InputMove, ETriggerEvent::Triggered, this, &ASuperFoxSpaceshipPawn::Move);
		playerEIcomponent->BindAction(InputFire, ETriggerEvent::Triggered, this, &ASuperFoxSpaceshipPawn::Fire);
	}
}

void ASuperFoxSpaceshipPawn::Move(const FInputActionValue& Value)
{
	FVector2D screenLocation = GetWorldLocationToScreen();
	FVector2D input = Value.Get<FVector2D>();
	
	if (screenLocation.X < TrackInset.X && input.X < 0) {
		SpaceshipMovement->Velocity.X = LerpSpaceshipVelocityToZero().X;
	} 
	else if (screenLocation.X > ViewportMaxX && input.X > 0) {
		SpaceshipMovement->Velocity.X = LerpSpaceshipVelocityToZero().X;
	}
	else {
		SpaceshipMovement->AddInputVector(GetActorRightVector() * input.X);
	}

	if (screenLocation.Y < TrackInset.Y && input.Y > 0) {
		SpaceshipMovement->Velocity.Y = LerpSpaceshipVelocityToZero().Y;
	} 
	else if (screenLocation.Y > ViewportMaxY && input.Y < 0) {
		SpaceshipMovement->Velocity.Y = LerpSpaceshipVelocityToZero().Y;
	}
	else {
		SpaceshipMovement->AddInputVector(GetActorUpVector() * input.Y);
	}

}

void ASuperFoxSpaceshipPawn::Fire(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("fire pressed"));

}

FVector2D ASuperFoxSpaceshipPawn::GetWorldLocationToScreen() const
{
	FVector2D screenLocation;
	APlayerController* playerController = Cast<APlayerController>(GetController());

	if (playerController == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("ASuperFoxSpaceshipPawn::GetWorldLocationToScreen - playerController is nullprt"));
		return screenLocation;
	}

	playerController->ProjectWorldLocationToScreen(StaticBaseMesh->GetComponentLocation(), screenLocation);
	return screenLocation;
}

FVector ASuperFoxSpaceshipPawn::LerpSpaceshipVelocityToZero() const
{
	FVector newSpeed = FMath::Lerp(SpaceshipMovement->Velocity, 0.0f, AxisDecelerationOnScreenEdges * UGameplayStatics::GetWorldDeltaSeconds(this));
	return newSpeed;
}

void ASuperFoxSpaceshipPawn::SpaceshipTiltAnimation()
{
	const float MAX_ROTATION_PITCH = 0.15f;
	const float MAX_ROTATION_YAW = 0.8f;
	const float interpolationSpeed = 500.f;
	FRotator currentRotation = StaticBaseMesh->GetComponentRotation();

	float roll = 0.f;
	float pitch = 0.f;


	// reset to original position
	if (FMath::IsNearlyZero(SpaceshipMovement->Velocity.Z)) {
		float targetPitch = (currentRotation.Pitch - InitialSpaceshipRotation.Pitch) * -1;
		pitch = FMath::FInterpTo(InitialSpaceshipRotation.Pitch, currentRotation.Pitch, UGameplayStatics::GetWorldDeltaSeconds(this), interpolationSpeed);

		UE_LOG(LogTemp, Warning, TEXT("spaceship yaw:%f pitch:%f roll:%f"), currentRotation.Yaw, currentRotation.Pitch, currentRotation.Roll);
		UE_LOG(LogTemp, Warning, TEXT("initial spaceship yaw:%f pitch:%f roll:%f"), InitialSpaceshipRotation.Yaw, InitialSpaceshipRotation.Pitch, InitialSpaceshipRotation.Roll);
		UE_LOG(LogTemp, Warning, TEXT("targetPitch:%f"), targetPitch);
		UE_LOG(LogTemp, Warning, TEXT("reset pitch:%f"), pitch);

		FRotator rotationAmount = currentRotation + FRotator(pitch, 0.f, 0.f);
		StaticBaseMesh->SetWorldRotation(rotationAmount);

		return;
	}

	pitch = FMath::FInterpTo(currentRotation.Pitch, MAX_ROTATION_YAW * SpaceshipMovement->GetLastInputVector().Z, UGameplayStatics::GetWorldDeltaSeconds(this), interpolationSpeed);
	roll = FMath::FInterpTo(currentRotation.Roll, MAX_ROTATION_PITCH * SpaceshipMovement->GetLastInputVector().X, UGameplayStatics::GetWorldDeltaSeconds(this), interpolationSpeed);
	FRotator rotationAmount = currentRotation + FRotator(pitch, 0.f, roll);

	UE_LOG(LogTemp, Warning, TEXT("last input vector x:%f y:%f z:%f"), SpaceshipMovement->GetLastInputVector().X, SpaceshipMovement->GetLastInputVector().Y, SpaceshipMovement->GetLastInputVector().Z);
	UE_LOG(LogTemp, Warning, TEXT("pitch:%f roll:%f"), pitch, roll);
	UE_LOG(LogTemp, Warning, TEXT("spaceship yaw:%f pitch:%f roll:%f"), currentRotation.Yaw, currentRotation.Pitch, currentRotation.Roll);

	StaticBaseMesh->SetWorldRotation(rotationAmount);
}