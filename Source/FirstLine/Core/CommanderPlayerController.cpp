// Fill out your copyright notice in the Description page of Project Settings.


#include "CommanderPlayerController.h"
#include "Interface/SelectionComponent.h"

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
			MovementVector *= DeltaTime;
			ControlledPawn->AddActorWorldOffset(FVector(MovementVector.X, MovementVector.Y, 0.0f));
		}
		MovementVector = FVector2D::ZeroVector;
	}

	if (bIsSelecting)
	{
		OnSelectUpdated();
	}
}

void ACommanderPlayerController::MoveCameraByInput(const FVector2D& Value)
{
	if (!IsLocalController()) return;
	MovementVector = Value * ScrollSpeed;
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
			MovementVector += Movement * ScrollSpeed;
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
	if (!IsLocalController() || !SelectionComponent) return;	

	FVector2D MousePos;
	if (GetMousePosition(MousePos.X, MousePos.Y))
	{
		bIsSelecting=true;
		SelectionComponent->StartSelection(MousePos);
	}
}

void ACommanderPlayerController::OnSelectUpdated()
{
	if (!IsLocalController() || !SelectionComponent) return;

	FVector2D MousePos;
	if (GetMousePosition(MousePos.X, MousePos.Y))
	{
		SelectionComponent->UpdateSelection(MousePos);
	}
}

void ACommanderPlayerController::OnSelectReleased()
{
	if (!IsLocalController() || !SelectionComponent) return;

	bIsSelecting=false;

	SelectionComponent->EndSelection();
}
