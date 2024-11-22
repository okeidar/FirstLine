 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_PeriodicSpawn.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTLINE_API UGA_PeriodicSpawn : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_PeriodicSpawn();

	
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayTagContainer* SourceTags = nullptr, 
		const FGameplayTagContainer* TargetTags = nullptr, 
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UPROPERTY(EditDefaultsOnly, Category = "Production")
	TSubclassOf<APawn> PeasantClass;

	UPROPERTY(EditDefaultsOnly, Category = "Production")
	float SpawnRadius = 300.0f;

private:
	UPROPERTY()
	AActor* TownHallRef;
	
};
