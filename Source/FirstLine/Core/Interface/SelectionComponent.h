// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, const TArray<AActor*>&, SelectedActors);

/**
 * Component that handles RTS-style unit selection logic
 * Handles both single-unit and box selection
 * Uses physics traces to find selectable units
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIRSTLINE_API USelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USelectionComponent();

	/** Handles single-click selection at a world location */
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void HandleClickSelection(const FVector& WorldLocation);

	/** Handles box selection between two world points */
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void HandleBoxSelection(const FVector& BoxStart, const FVector& BoxEnd);

	/** Updates the current selection, handling selection/deselection events */
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void UpdateSelection(const TArray<AActor*>& NewSelection);

	/** Clears the current selection */
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void ClearSelection();

	/** Returns the currently selected actors */
	UFUNCTION(BlueprintPure, Category = "Selection")
	const TArray<AActor*>& GetSelectedActors() const { return SelectedActors; }

	/** Broadcast when selection changes */
	UPROPERTY(BlueprintAssignable, Category = "Selection")
	FOnSelectionChanged OnSelectionChanged;

private:
	/** Currently selected actors */
	UPROPERTY()
	TArray<AActor*> SelectedActors;

	/** Performs a box trace to find selectable actors */
	bool GetSelectableActorsInBox(const FVector& BoxStart, const FVector& BoxEnd, TArray<AActor*>& OutActors) const;

	/** Performs a line trace to find a selectable actor */
	bool GetSelectableActorAtLocation(const FVector& Location, AActor*& OutActor) const;
};
