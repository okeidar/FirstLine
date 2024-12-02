// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commands/CommandTypes.h"
#include "GameFramework/PlayerController.h"
#include "CommanderPlayerController.generated.h"

class USelectionComponent;
class UCommandSystemComponent;

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

	// Input handlers - to be bound in Blueprint using Enhanced Input
	UFUNCTION(BlueprintCallable)
	void OnSelectPressed();
	UFUNCTION(BlueprintCallable) 
	void OnSelectReleased();
	UFUNCTION(BlueprintCallable)
	void OnSelectUpdated();
	UFUNCTION(BlueprintCallable)
	void OnCommandPressed();

	
	UFUNCTION(BlueprintCallable)
	void ExecuteCustomCommand(FCommandData Command);

	/** Component that handles all selection logic */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USelectionComponent> SelectionComponent;

	/** Component that handles command system logic */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCommandSystemComponent> CommandSystemComponent;

	// Called every frame to update cursor based on command context
	void UpdateCommandContext();

	// Cache the last command data to avoid unnecessary cursor updates
	UPROPERTY()
	FCommandData LastCommandData;

	UPROPERTY()
	bool bIsBuildingMode = false;

	void SetBuildingMode(bool bEnabled);
	bool IsBuildingMode() const { return bIsBuildingMode; }

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
