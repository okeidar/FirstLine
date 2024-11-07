// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/SelectionComponent.h"
#include "CommanderPlayerController.generated.h"

/**
 * RTS-style player controller that handles camera movement and unit selection input
 */
UCLASS()
class FIRSTLINE_API ACommanderPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ACommanderPlayerController();
    
	virtual void Tick(float DeltaTime) override;
	
	/** Called from Enhanced Input to move camera based on WASD input */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void MoveCameraByInput(const FVector2D& Value);

protected:

	/** Camera movement speed when using keyboard or edge scrolling */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ScrollSpeed = 500.0f;
    
	/** Distance from screen edge that triggers edge scrolling */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float EdgeScrollThreshold = 20.0f;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Blueprint events for selection box visualization
	UFUNCTION(BlueprintImplementableEvent, Category = "Selection")
	void OnSelectionStarted(const FVector2D& ScreenPosition);

	UFUNCTION(BlueprintImplementableEvent, Category = "Selection")
	void OnSelectionUpdated(const FVector2D& CurrentScreenPosition);

	UFUNCTION(BlueprintImplementableEvent, Category = "Selection")
	void OnSelectionEnded();

	// Input handlers - to be bound in Blueprint using Enhanced Input
	UFUNCTION(BlueprintCallable)
	void OnSelectPressed();
	UFUNCTION(BlueprintCallable) 
	void OnSelectReleased();

	/** Component that handles all selection logic */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USelectionComponent> SelectionComponent;

private:

	/** Current frame's camera movement vector */
	FVector2D MovementVector{0.f,0.f};

	/** Handles camera movement when mouse is near screen edges */
	void EdgeScroll();
	
	/** Whether we're currently in the process of box selection */
	bool bIsSelecting;
	
	/** Screen position where the current selection started */
	FVector2D SelectionStart;
};
