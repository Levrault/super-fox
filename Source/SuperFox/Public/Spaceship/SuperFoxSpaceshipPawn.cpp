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


ASuperFoxSpaceshipPawn::ASuperFoxSpaceshipPawn()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;
	
	StaticBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	StaticBaseMesh->SetupAttachment(RootComponent);

	SpaceshipMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
	SpaceshipMovement->SetUpdatedComponent(StaticBaseMesh);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arms");
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
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

	SpaceshipMovement->MaxSpeed = AxisMovementSpeed;

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


	if (SpaceshipMovement->MaxSpeed != AxisMovementSpeed) {
		SpaceshipMovement->MaxSpeed = AxisMovementSpeed;
	}

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

	APlayerController* playerController = Cast<APlayerController>(GetController());
	
	FVector2D screenLocation;
	playerController->ProjectWorldLocationToScreen(StaticBaseMesh->GetComponentLocation(), screenLocation);
	UE_LOG(LogTemp, Warning, TEXT("Screen Location: X=%f, Y=%f"), screenLocation.X, screenLocation.Y);

	FVector2D input = Value.Get<FVector2D>();
	if (screenLocation.X <= TrackInset.X) {
		SpaceshipMovement->MaxSpeed = AxisMovementSpeed / 3;
		SpaceshipMovement->AddInputVector(GetActorRightVector());
		return;
	}

	if (screenLocation.X >= ViewportMaxX) {
		SpaceshipMovement->MaxSpeed = AxisMovementSpeed / 3;
		SpaceshipMovement->AddInputVector(GetActorRightVector() * -1);
		return;
	}

	if (screenLocation.Y <= TrackInset.Y) {
		SpaceshipMovement->MaxSpeed = AxisMovementSpeed / 3;
		SpaceshipMovement->AddInputVector(GetActorUpVector() * -1);
		return;
	}

	if (screenLocation.Y > ViewportMaxY) {
		SpaceshipMovement->MaxSpeed = AxisMovementSpeed / 3;
		SpaceshipMovement->AddInputVector(GetActorUpVector());
		return;
	}

	SpaceshipMovement->AddInputVector(GetActorRightVector() * input.X);
	SpaceshipMovement->AddInputVector(GetActorUpVector() * input.Y);

}


void  ASuperFoxSpaceshipPawn::Fire(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("fire pressed"));

}