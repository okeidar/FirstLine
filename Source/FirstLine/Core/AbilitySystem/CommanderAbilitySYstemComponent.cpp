// Fill out your copyright notice in the Description page of Project Settings.


#include "CommanderAbilitySYstemComponent.h"

#include "FirstLineGameplayTags.h"


UCommanderAbilitySYstemComponent::UCommanderAbilitySYstemComponent()
{
	OnGameplayEffectRemovedDelegateHandle = OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::OnGameplayEffectRemoved);
}

void UCommanderAbilitySYstemComponent::OnGameplayEffectRemoved(const FActiveGameplayEffect& RemovedEffect)
{
	// Check if this was a cooldown effect
	if (RemovedEffect.Spec.Def->GetAssetTags().HasTag(FFirstLineGameplayTags::Get().Ability_Cooldown))
	{
		// Get the ability that created this cooldown
		if (const UGameplayAbility* Ability = RemovedEffect.Spec.GetEffectContext().GetAbility())
		{
			if (Ability->GetAssetTags().HasTag(FFirstLineGameplayTags::Get().Ability_AutoRetrigger))
			{
				// Find the spec for this ability
				if (const FGameplayAbilitySpec* FoundSpec = FindAbilitySpecFromClass(Ability->GetClass()))
				{
					TryActivateAbility(FoundSpec->Handle, false);
				}
			}
		}
	}
}

void UCommanderAbilitySYstemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	UE_LOG(LogTemp, Log, TEXT("Granting Ability %s"), *AbilitySpec.Ability->GetName());
	Super::OnGiveAbility(AbilitySpec);
	
	if (AbilitySpec.Ability->GetAssetTags().HasTag(FFirstLineGameplayTags::Get().Ability_AutoRetrigger))
	{
		UE_LOG(LogTemp, Log, TEXT("Activating Ability %s"), *AbilitySpec.Ability->GetName());
		TryActivateAbility(AbilitySpec.Handle, false);
	}
	
}
