// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectionComponent.generated.h"

class ACommanderHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, const TArray<AActor*>&, SelectedActors);

/**
 * Component that handles RTS-style unit selection logic
 * Handles both single-unit and box selection
 * Uses physics traces to find selectable units
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIRSTLINE_API USelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USelectionComponent();
	virtual void BeginPlay() override;

	/** Handles single-click selection at a world location */
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void StartSelection(const FVector2D& ScreenPosition);

	/** Handles box selection between two world points */
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void UpdateSelection(const FVector2D& CurrentScreenPos);

	/** Clears the current selection */
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void EndSelection();

	/** Updates the current selection, handling selection/deselection events */
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void UpdateSelectedActors(const TArray<AActor*>& ActorsToSelect);

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

	UPROPERTY()
	TObjectPtr<ACommanderHUD> CachedHUD;

	FVector2D SelectionStart;
	FVector2D CurrentPosition;
};
