// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_PeriodicSpawn.h"

#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "AbilitySystem/FirstLineGameplayTags.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "FirstLine/Core/AbilitySystem/CommanderAttributeSet.h"

UGA_PeriodicSpawn::UGA_PeriodicSpawn()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	bServerRespectsRemoteAbilityCancellation = false;
	SetAssetTags(FFirstLineGameplayTags::Get().Ability_AutoRetrigger.GetSingleTagContainer());	 
}



void UGA_PeriodicSpawn::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Log, TEXT("Activating Ability %s"), *GetName());

	if (!ActorInfo->IsNetAuthority())
    {
		UE_LOG(LogTemp, Log, TEXT("Not net authority, ending ability %s"), *GetName());
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to commit ability %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Find TownHall in world (for testing only)
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(FName("TownHall")) )
		{
			TownHallRef = *It;
			break;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("TownHallRef: %s"), *TownHallRef->GetName());

	if (!TownHallRef || !PeasantClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	FVector SpawnLocation;
	FNavLocation NavLocation;
	UNavigationSystemV1* NavSystem =FNavigationSystem::GetCurrent<UNavigationSystemV1>(this) ;//UNavigationSystem::GetCurrent(GetWorld());::GetNavigationSystem(GetWorld());

	if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(TownHallRef->GetActorLocation(), SpawnRadius, NavLocation))
	{
		SpawnLocation = NavLocation.Location;
	}
	else
	{
		FVector RandomOffset = FMath::VRand() * SpawnRadius;
		RandomOffset.Z = TownHallRef->GetActorLocation().Z;  		
		SpawnLocation = TownHallRef->GetActorLocation() + RandomOffset;
	}

	// Spawn the peasant

	APawn* NewPeasant = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(),PeasantClass,nullptr, SpawnLocation, FRotator::ZeroRotator, true, ActorInfo->OwnerActor.Get());



	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UGA_PeriodicSpawn::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayTagContainer* SourceTags, 
	const FGameplayTagContainer* TargetTags, 
	OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const UCommanderAttributeSet* AttributeSet = Cast<UCommanderAttributeSet>(ActorInfo->AbilitySystemComponent->GetAttributeSet(UCommanderAttributeSet::StaticClass()));
	if (!AttributeSet)
	{
		return false;
	}

	return AttributeSet->GetCurrentPopulation() < AttributeSet->GetMaxPopulation();
}