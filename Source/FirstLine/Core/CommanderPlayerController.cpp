// Fill out your copyright notice in the Description page of Project Settings.


#include "CommanderPlayerController.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Interface/ISelectable.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h" // For DeprojectScreenToWorld

ACommanderPlayerController::ACommanderPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
    
	PrimaryActorTick.bCanEverTick = true;

	
	SelectionComponent = CreateDefaultSubobject<USelectionComponent>(TEXT("SelectionComponent"));
}

void ACommanderPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	EdgeScroll();
	if (!MovementVector.IsZero())
	{
		if (APawn* ControlledPawn = GetPawn())
		{
			MovementVector*=DeltaTime;
			ControlledPawn->AddActorWorldOffset(FVector(MovementVector.X, MovementVector.Y, 0.0f));
		}
		MovementVector = FVector2D::ZeroVector;
	}
    
	if (bIsSelecting)
	{
		FVector2D CurrentMousePos;
		if (GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y))
		{
			OnSelectionUpdated(CurrentMousePos);
		}
	}
}

void ACommanderPlayerController::MoveCameraByInput(const FVector2D& Value)
{
	if (!IsLocalController()) return;	
	MovementVector = Value*ScrollSpeed ;	
}

void ACommanderPlayerController::EdgeScroll()
{
	if (!IsLocalController()) return;

	float MouseX = 0.0f;
	float MouseY = 0.0f;
    
	if (GetMousePosition(MouseX, MouseY))
	{
		int32 ViewportSizeX, ViewportSizeY;
		GetViewportSize(ViewportSizeX, ViewportSizeY);
        
		FVector2D Movement(0.0f, 0.0f);
        
		if (MouseX <= EdgeScrollThreshold) Movement.Y = 1.0f;
		else if (MouseX >= ViewportSizeX - EdgeScrollThreshold) Movement.Y = -1.0f;
		if (MouseY <= EdgeScrollThreshold) Movement.X = -1.0f;
		else if (MouseY >= ViewportSizeY - EdgeScrollThreshold) Movement.X = 1.0f;

		if (!Movement.IsZero())
		{
			MovementVector += Movement*ScrollSpeed;			
		}
	}
}

void ACommanderPlayerController::BeginPlay()
{
	Super::BeginPlay();
    
}

void ACommanderPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Note: Bind inputs in Blueprint using Enhanced Input System
}

void ACommanderPlayerController::OnSelectPressed()
{
	if (!IsLocalController()) return;

	bIsSelecting = true;
	FVector2D MousePos;
	if (GetMousePosition(MousePos.X, MousePos.Y))
	{
		SelectionStart = MousePos;
		OnSelectionStarted(MousePos);
	}
}

void ACommanderPlayerController::OnSelectReleased()
{
	if (!IsLocalController() || !bIsSelecting || !SelectionComponent) return;

	FVector2D CurrentMousePos;
	if (!GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y))
		return;

	// Convert screen positions to world space
	FVector WorldStart, WorldStartDir;
	FVector WorldEnd, WorldEndDir;
	
	// Get world positions by deprojecting screen coordinates
	DeprojectScreenPositionToWorld(
		SelectionStart.X, SelectionStart.Y,
		WorldStart, WorldStartDir);
		
	DeprojectScreenPositionToWorld(
		CurrentMousePos.X, CurrentMousePos.Y,
		WorldEnd, WorldEndDir);

	float DragDistance = FVector2D::Distance(SelectionStart, CurrentMousePos);
    
	if (DragDistance < 3.0f)
	{
		SelectionComponent->HandleClickSelection(WorldEnd);
	}
	else
	{
		SelectionComponent->HandleBoxSelection(WorldStart, WorldEnd);
	}

	bIsSelecting = false;
	OnSelectionEnded();
}
