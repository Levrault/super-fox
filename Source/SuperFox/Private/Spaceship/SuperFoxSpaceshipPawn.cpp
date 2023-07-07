#include "Spaceship/SuperFoxSpaceshipPawn.h"
#include "Level/SplineTrack.h"

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
