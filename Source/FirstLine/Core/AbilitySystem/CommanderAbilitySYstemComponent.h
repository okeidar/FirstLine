// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "CommanderAbilitySYstemComponent.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTLINE_API UCommanderAbilitySYstemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UCommanderAbilitySYstemComponent();
protected:
	void OnGameplayEffectRemoved(const FActiveGameplayEffect& RemovedEffect);
	FDelegateHandle OnGameplayEffectRemovedDelegateHandle;
	
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	
};

