// Fill out your copyright notice in the Description page of Project Settings.


#include "CommanderPlayerController.h"

#include "AbilitySystemComponent.h"
#include "CommanderPlayerSTate.h"
#include "Interface/SelectionComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Commands/CommandSystemComponent.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h" // For DeprojectScreenToWorld


ACommanderPlayerController::ACommanderPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	PrimaryActorTick.bCanEverTick = true;


	SelectionComponent = CreateDefaultSubobject<USelectionComponent>(TEXT("SelectionComponent"));
	CommandSystemComponent= CreateDefaultSubobject<UCommandSystemComponent>(TEXT("CommandSystem"));
}

void ACommanderPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EdgeScroll();
	UpdateCommandContext();

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

void ACommanderPlayerController::UpdateCommandContext()
{
	if (!IsLocalController() || !CommandSystemComponent) return;

	// Reset cursor if no units selected
	if (SelectionComponent->GetSelectedActors().Num() == 0)
	{
		
		LastCommandData = FCommandData();
		CurrentMouseCursor = EMouseCursor::Default;
		
		return;
	}

	// Get actor under cursor
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	// Get available command based on context
	const FCommandData CommandData = CommandSystemComponent->GetAvailableCommand(
		SelectionComponent->GetSelectedActors(), 
		HitResult.GetActor(),
		HitResult.Location
	);

	// Update cursor if command changed
	if (CommandData.CommandTag != LastCommandData.CommandTag)
	{
		LastCommandData = CommandData;
		if (CommandData.CursorTexture.IsValid())
		{
			// Set custom cursor
			CurrentMouseCursor = EMouseCursor::Custom;
			
			//TODO: Set Cursor
			UE_LOG(LogTemp, Warning, TEXT("Mouse cursor changed"));
			
		}
		else
		{
			// Reset to default cursor
			CurrentMouseCursor = EMouseCursor::Default;
		}
	}
}

void ACommanderPlayerController::OnCommandPressed()
{
	if (!IsLocalController() || !CommandSystemComponent || !LastCommandData.CommandTag.IsValid()) return;

	// Get target location/actor
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		// Create event data wrapper
		UCommandEventDataWrapper* EventDataWrapper = NewObject<UCommandEventDataWrapper>();
		EventDataWrapper->EventData.TargetLocation = HitResult.Location;
		EventDataWrapper->EventData.TargetActor = HitResult.GetActor();
		EventDataWrapper->EventData.SelectedUnits = SelectionComponent->GetSelectedActors();

		// Trigger ability through GAS
		if (ACommanderPlayerSTate* PS = GetPlayerState<ACommanderPlayerSTate>())
		{
			if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
			{
				FGameplayEventData Payload;
				Payload.EventTag = LastCommandData.AbilityTriggerTag;
				Payload.Target = HitResult.GetActor();
				Payload.OptionalObject = EventDataWrapper;

				ASC->HandleGameplayEvent(LastCommandData.AbilityTriggerTag, &Payload);
			}
		}
	}
}