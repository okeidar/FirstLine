// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionComponent.h"
#include "ISelectable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"


USelectionComponent::USelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}




bool USelectionComponent::GetSelectableActorsInBox(const FVector& BoxStart, const FVector& BoxEnd, TArray<AActor*>& OutActors) const
{
	if (UWorld* World = GetWorld())
	{
		// Log box dimensions
		UE_LOG(LogTemp, Log, TEXT("Attempting box selection: Start=%s, End=%s"), 
			*BoxStart.ToString(), *BoxEnd.ToString());

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;
		
		FVector BoxExtent = (BoxEnd - BoxStart).GetAbs() * 0.5f;
		FVector BoxCenter = (BoxEnd + BoxStart) * 0.5f;
		
		// Debug draw the box
		DrawDebugBox(World, BoxCenter, BoxExtent, FQuat::Identity, 
			FColor::Red, false, 2.0f, 0, 2.0f);

		TArray<FOverlapResult> OverlapResults;
		if (World->OverlapMultiByChannel(
			OverlapResults,
				BoxCenter,
				FQuat::Identity,
				ECC_Visibility,
				FCollisionShape::MakeBox(BoxExtent),
				QueryParams))
		{
			for (const FOverlapResult& Overlap : OverlapResults)
			{
				if (AActor* Actor = Overlap.GetActor())
				{
					UE_LOG(LogTemp, Log, TEXT("Found actor in box: %s"), *Actor->GetName());
					
					if (IISelectable* Selectable = Cast<IISelectable>(Actor))
					{
						UE_LOG(LogTemp, Log, TEXT("Actor implements ISelectable"));
						if (Selectable->Execute_CanBeSelected(Actor))
						{
							UE_LOG(LogTemp, Log, TEXT("Actor can be selected"));
							OutActors.AddUnique(Actor);
						}
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("Actor does not implement ISelectable"));
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("No overlaps found in box"));
		}
	}
	return OutActors.Num() > 0;
}

void USelectionComponent::UpdateSelection(const TArray<AActor*>& NewSelection)
{
	// First pass: Handle deselection
	// We deselect any currently selected actors that aren't in the new selection
	for (AActor* Actor : SelectedActors)
	{
		if (Actor && !NewSelection.Contains(Actor))
		{
			if (IISelectable* Selectable = Cast<IISelectable>(Actor))
			{
				UE_LOG(LogTemp, Verbose, TEXT("Deselecting actor: %s"), *Actor->GetName());
				Selectable->Execute_OnDeselected(Actor);
			}
		}
	}

	// Second pass: Handle new selections
	// We select any actors in the new selection that weren't already selected
	for (AActor* Actor : NewSelection)
	{
		if (Actor && !SelectedActors.Contains(Actor))
		{
			if (IISelectable* Selectable = Cast<IISelectable>(Actor))
			{
				UE_LOG(LogTemp, Verbose, TEXT("Selecting actor: %s"), *Actor->GetName());
				Selectable->Execute_OnSelected(Actor);
			}
		}
	}

	SelectedActors = NewSelection;
	OnSelectionChanged.Broadcast(SelectedActors);
}

void USelectionComponent::ClearSelection()
{
	UpdateSelection(TArray<AActor*>());
}

bool USelectionComponent::GetSelectableActorAtLocation(const FVector& Location, AActor*& OutActor) const
{
	if (UWorld* World = GetWorld())
	{
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;  // Use simple collision for performance

		FHitResult HitResult;
		// Perform a line trace downward from the click position
		// This is useful for clicking on ground-based units
		if (World->LineTraceSingleByChannel(
			HitResult,
			Location,                           // Start at click position
			Location + FVector(0, 0, -10000),   // Trace downward for 10000 units
			ECC_Visibility,
			QueryParams))
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				// Same selection validation as box selection
				if (IISelectable* Selectable = Cast<IISelectable>(HitActor))
				{
					if (Selectable->Execute_CanBeSelected(HitActor))
					{
						OutActor = HitActor;
						return true;
					}
				}
			}
		}
	}
	return false;
}

void USelectionComponent::HandleClickSelection(const FVector& WorldLocation)
{
    AActor* ClickedActor;
    if (GetSelectableActorAtLocation(WorldLocation, ClickedActor))
    {
        UE_LOG(LogTemp, Log, TEXT("Click selected actor: %s"), *ClickedActor->GetName());
        UpdateSelection({ClickedActor});
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Click selection found no actors, clearing selection"));
        ClearSelection();
    }
}

void USelectionComponent::HandleBoxSelection(const FVector& BoxStart, const FVector& BoxEnd)
{
    TArray<AActor*> ActorsInBox;
    GetSelectableActorsInBox(BoxStart, BoxEnd, ActorsInBox);
    
    UE_LOG(LogTemp, Log, TEXT("Box selection found %d actors"), ActorsInBox.Num());
    UpdateSelection(ActorsInBox);
}

