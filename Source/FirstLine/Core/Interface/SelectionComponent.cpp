// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionComponent.h"
#include "CommanderHUD.h"
#include "ISelectable.h"
#include "GameFramework/PlayerController.h"


USelectionComponent::USelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void USelectionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		CachedHUD = Cast<ACommanderHUD>(PC->GetHUD());
	}
}

void USelectionComponent::StartSelection(const FVector2D& ScreenPosition)
{
	if (!CachedHUD) return;

	SelectionStart = ScreenPosition;
	CurrentPosition = ScreenPosition;
	CachedHUD->StartSelection(ScreenPosition);
}

void USelectionComponent::UpdateSelection(const FVector2D& CurrentScreenPos)
{
	if (!CachedHUD) return;

	CurrentPosition = CurrentScreenPos;
	CachedHUD->UpdateSelection(CurrentScreenPos);
}

void USelectionComponent::EndSelection()
{
	if (!CachedHUD) return;

	TArray<AActor*> ActorsInBox;
	CachedHUD->EndSelection(ActorsInBox);
	UpdateSelectedActors(ActorsInBox);
}

void USelectionComponent::UpdateSelectedActors(const TArray<AActor*>& ActorsToSelect)
{
	// Deselect current selection
	for (AActor* Actor : SelectedActors)
	{
		if (Actor && !ActorsToSelect.Contains(Actor))
		{
			if (Actor->Implements<UISelectable>())
			{
				IISelectable::Execute_OnDeselected(Actor);
			}
		}
	}

	// Select new actors
	for (AActor* Actor : ActorsToSelect)
	{
		if (Actor && !SelectedActors.Contains(Actor))
		{
			if (Actor->Implements<UISelectable>())
			{
				if (IISelectable::Execute_CanBeSelected(Actor))
				{
					IISelectable::Execute_OnSelected(Actor);
				}
			}
		}
	}

	SelectedActors = ActorsToSelect;
	OnSelectionChanged.Broadcast(SelectedActors);
}

