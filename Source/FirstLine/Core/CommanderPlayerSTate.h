// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CommanderPlayerSTate.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class FIRSTLINE_API ACommanderPlayerSTate : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
    ACommanderPlayerSTate();

    // IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

    // Getter for the AttributeSet
    class UCommanderAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
    UPROPERTY()
    class UAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY()
    class UCommanderAttributeSet* AttributeSet;

	virtual void BeginPlay() override;

	
};
