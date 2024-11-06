// Fill out your copyright notice in the Description page of Project Settings.


#include "CommanderPlayerSTate.h"
#include "AbilitySystem/CommanderAbilitySYstemComponent.h"
#include "AbilitySystem/CommanderAttributeSet.h"

ACommanderPlayerSTate::ACommanderPlayerSTate()
{
    AbilitySystemComponent = CreateDefaultSubobject< UCommanderAbilitySYstemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UCommanderAttributeSet>(TEXT("AttributeSet"));

    SetNetUpdateFrequency(100.0f);
}

void ACommanderPlayerSTate::BeginPlay()
{
    Super::BeginPlay();
    
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
    }
}