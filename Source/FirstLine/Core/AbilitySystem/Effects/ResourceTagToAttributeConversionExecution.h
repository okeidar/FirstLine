// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "ResourceTagToAttributeConversionExecution.generated.h"


/**
 * 
 */
UCLASS()
class FIRSTLINE_API UResourceTagToAttributeConversionExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UResourceTagToAttributeConversionExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
