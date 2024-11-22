// Fill out your copyright notice in the Description page of Project Settings.


#include "CommanderAttributeSet.h"
#include "Net/UnrealNetwork.h"

UCommanderAttributeSet::UCommanderAttributeSet()
{
	CurrentPopulation = 0.0f;
	MaxPopulation = 100.0f;
}

void UCommanderAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UCommanderAttributeSet, CurrentPopulation, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCommanderAttributeSet, MaxPopulation, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCommanderAttributeSet, Wood, COND_None, REPNOTIFY_Always);
}

void UCommanderAttributeSet::OnRep_CurrentPopulation(const FGameplayAttributeData& OldValue)
{	
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCommanderAttributeSet, CurrentPopulation, OldValue);
}

void UCommanderAttributeSet::OnRep_Wood(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCommanderAttributeSet, Wood, OldValue);
}

void UCommanderAttributeSet::OnRep_MaxPopulation(const FGameplayAttributeData& OldValue)
{	
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCommanderAttributeSet, MaxPopulation, OldValue);
}
